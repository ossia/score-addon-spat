#include <QCoreApplication>
#include <QPointer>
#include <QThreadPool>

#include <Gris/LayoutCache.hpp>

#include <map>
#include <vector>

namespace Gris
{
namespace
{
struct Waiter
{
  QPointer<QObject> context;
  std::function<void(LayoutPtr)> done;
};

struct Entry
{
  std::weak_ptr<Layout const> layout;
  std::vector<Waiter> waiters;
  bool building{};
};

std::map<std::string, Entry>& entries()
{
  static std::map<std::string, Entry> map;
  return map;
}

void prune()
{
  auto& map = entries();
  for(auto it = map.begin(); it != map.end();)
    it = (!it->second.building && it->second.layout.expired()) ? map.erase(it)
                                                               : std::next(it);
}

void finish(std::string const& key, LayoutPtr const& layout)
{
  auto& map = entries();
  auto it = map.find(key);
  if(it == map.end())
    return;

  auto waiters = std::move(it->second.waiters);
  it->second.waiters.clear();
  it->second.building = false;
  it->second.layout = layout;

  for(auto& w : waiters)
    if(w.context)
      w.done(layout);

  prune();
}
}

LayoutPtr findLayout(std::string const& key)
{
  auto& map = entries();
  if(auto it = map.find(key); it != map.end())
    return it->second.layout.lock();
  return {};
}

void requestLayout(
    std::string const& key, SpeakerSetup setup, QObject* context,
    std::function<void(LayoutPtr)> done)
{
  if(auto layout = findLayout(key))
  {
    done(std::move(layout));
    return;
  }

  auto& entry = entries()[key];
  entry.waiters.push_back({context, std::move(done)});
  if(entry.building)
    return;
  entry.building = true;

  QThreadPool::globalInstance()->start([key, setup = std::move(setup)]() mutable {
    auto layout = Layout::make(std::move(setup));
    QMetaObject::invokeMethod(qApp, [key, layout = std::move(layout)] {
      finish(key, layout);
    }, Qt::QueuedConnection);
  });
}
}

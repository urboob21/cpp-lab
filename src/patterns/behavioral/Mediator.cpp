// Mediator is a behavioral design pattern that lets you reduce chaotic dependencies between objects.
// The pattern restricts direct communications between the objects and forces them to collaborate only via a mediator object.
// Usage examples: The most popular usage of the Mediator pattern in C++ code is facilitating communications between GUI components of an app.
// The synonym of the Mediator is the `Controller` part of MVC pattern.
// Appicability:
// (*)   when your collection has a complex data structure under the hood, but you want to hide its complexity from clients (either for convenience or security reasons).
// (**)  when you can’t reuse a component in a different program because it’s too dependent on other components.
// (***) when you find yourself creating tons of component subclasses just to reuse some basic behavior in various contexts.

// UML: docs/uml/patterns_behavioral_mediator.drawio.svg

#include <iostream>
#include <string>
#include <vector>
namespace
{
    namespace Mediator
    {

        enum class Event
        {
            CREATE = 0,
            READ,
            UPDATE,
            DELETE,
        };

        static inline const char *getEventName(const Event &e)
        {
            switch (e)
            {
            case Event::CREATE:
                return "CREATE";
            case Event::READ:
                return "READ";
            case Event::UPDATE:
                return "UPDATE";
            case Event::DELETE:
                return "DELETE";
            }
            return "invalid_event";
        }

        class IComponent
        {
        public:
            virtual ~IComponent() = default;

            virtual void send(const Event e) = 0;
            virtual void receive(const Event e) = 0;
        };

        /**
         * The Mediator interface declares methods of communication with components, which usually include just a single notification method.
         * Components may pass any context as arguments of this method, including their own objects,
         * but only in such a way that no coupling occurs between a receiving component and the sender’s class.
         */
        class IMediator
        {
        public:
            virtual ~IMediator() = default;
            virtual void registerComponent(IComponent *const c) = 0;
            virtual void notify(IComponent *sender, const Event &e) = 0;
        };

        /**
         * Concrete Mediators implement cooperative behavior by coordinating several components.
         * Concrete mediators often keep references to all components they manage and sometimes even manage their lifecycle.
         */
        class ComponentMediator : public IMediator
        {
        private:
            std::vector<IComponent *> m_components;

        public:
            void registerComponent(IComponent *const c) override
            {
                m_components.push_back(c);
            }

            void notify(IComponent *const sender, const Event &e) override
            {
                for (auto c : m_components)
                {
                    if (c != sender)
                    {
                        c->receive(e);
                    }
                }
            }
        };

        /**
         * Components are various classes that contain some business logic.
         * Each component has a reference to a mediator, declared with the type of the mediator interface.
         * The component isn’t aware of the actual class of the mediator, so you can reuse the component in other programs by linking it to a different mediator.
         */
        class AbstractComponent : public IComponent
        {
        private:
            const std::string m_id;

        protected:
            IMediator *m_mediator;
            void log(const Event &e, const std::string &msg) const
            {
                std::cout << "\t" + msg + "-id:" + m_id + "-event:" + getEventName(e) + "\n";
            }

        public:
            explicit AbstractComponent(const std::string &id, IMediator *const m = nullptr) : m_id{id}, m_mediator{m} {};
        };

        /**
         * Concrete Components implement various functionality. They don't depend on
         * other components. They also don't depend on any concrete mediator classes.
         */
        class ConreteComponent : public AbstractComponent
        {
        public:
            explicit ConreteComponent(const std::string &id, IMediator *const m = nullptr) : AbstractComponent{id, m} {}

            void send(const Event e) override
            {
                log(e, "[SEND]");
                if (m_mediator != nullptr)
                    m_mediator->notify(this, e);
            }

            void receive(const Event e) override
            {
                log(e, "[RECV]");
                // Additional handling logic can go here
            }
        };

        namespace Client
        {
            void clientCode(IComponent *comp)
            {
                comp->send(Event::READ);
            }
        }

        void run()
        {
            IMediator *mediator = new ComponentMediator();
            IComponent *c1 = new ConreteComponent("1763700876", mediator);
            IComponent *c2 = new ConreteComponent("1763700882", mediator);
            IComponent *c3 = new ConreteComponent("1763700899", mediator);
            IComponent *c4 = new ConreteComponent("1763700900", mediator);

            // Only c1, c3, c4 receive notifications.
            mediator->registerComponent(c1);
            mediator->registerComponent(c3);
            mediator->registerComponent(c4);

            // c2 triggers event => observed by others
            Client::clientCode(c2);

            delete mediator;
            delete c1;
            delete c2;
            delete c3;
            delete c4;
        }
    }
}

struct MediatorAutoRunner
{
    MediatorAutoRunner()
    {
        std::cout << "\n--- Mediator Pattern Example ---\n";
        Mediator::run();
    }
};

static MediatorAutoRunner instance;
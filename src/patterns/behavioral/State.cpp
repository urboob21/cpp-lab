// State is a behavioral design pattern that lets an object alter its behavior when its internal state changes.
// It appears as if the object changed its class.
// Appicability:
// (*)    when you have an object that behaves differently depending on its current state
//        , the number of states is enormous, and the state-specific code changes frequently.
// (**)   when you have a class polluted with massive conditionals that alter
//        how the class behaves according to the current values of the class’s fields.
// (***)  when you have a lot of duplicate code across similar states and transitions of a condition-based state machine.
// UML: docs/uml/patterns_behavioral_state.drawio.svg

#include <iostream>
#include <string>
namespace
{
    namespace State
    {
        class DeviceContext;

        /**
         * The State interface declares the state-specific methods.
         * These methods should make sense for all concrete states because you don’t want some of your states
         * to have useless methods that will never be called.
         */
        class IState
        {
        public:
            virtual ~IState() = default;
            virtual void setContext(DeviceContext *ctx) = 0;
            virtual void handle() = 0;
        };

        /*
         * To avoid duplication of similar code across multiple states, you may provide intermediate abstract classes
         * that encapsulate some common behavior.
         */
        class AbstractState : public IState
        {
        protected:
            DeviceContext *m_ctx;

        public:
            void setContext(DeviceContext *ctx) override
            {
                this->m_ctx = ctx;
            }
        };

        /**
         * Context stores a reference to one of the concrete state objects and delegates to it all state-specific work.
         * The context communicates with the state object via the state interface.
         * The context exposes a setter for passing it a new state object.
         */
        class DeviceContext
        {
        private:
            IState *m_state;

        public:
            explicit DeviceContext(IState *state) : m_state{nullptr}
            {
                this->changeState(state);
            }

            ~DeviceContext()
            {
                delete m_state;
            }

            void changeState(IState *state)
            {
                std::cout << "[DeviceContext]: Changing state\n";
                if (this->m_state != nullptr)
                {
                    delete this->m_state;
                }
                this->m_state = state;
                this->m_state->setContext(this);
            }

            void operation()
            {
                this->m_state->handle();
            }
        };

        /**
         * Concrete States provide their own implementations for the state-specific methods.
         */
        class IdeConcreteState : public AbstractState
        {
        public:
            void handle() override;
        };

        class ProcessingConcreteState : public AbstractState
        {
        public:
            void handle() override;
        };

        class ErrorConcreteState : public AbstractState
        {
        public:
            void handle() override
            {
                std::cout << "[Error] Device error! Reset required.\n";

                // After recover => go Idle
                this->m_ctx->changeState(new IdeConcreteState());
            }
        };

        void IdeConcreteState::handle()
        {
            std::cout << "[Ide] Device is idle. Waiting...\n";
            this->m_ctx->changeState(new ProcessingConcreteState());
        }

        void ProcessingConcreteState::handle()
        {
            std::cout << "[Processing] Processing data...\n";
            bool ok = true; // Example processing result
            static int index = 0;
            index++;
            if (index % 2 == 0)
            {
                ok = true;
            }
            else
            {
                ok = false;
            }
            if (ok)
            {
                // Back to idle after finishing job
                this->m_ctx->changeState(new IdeConcreteState());
            }
            else
            {
                this->m_ctx->changeState(new ErrorConcreteState());
            }
        }

        namespace Client
        {
            void clientCode(DeviceContext *const device)
            {
                device->operation();
            }
        }
        void run()
        {
            DeviceContext *device = new DeviceContext(new IdeConcreteState());
            for (int loopIdx = 0; loopIdx < 10; ++loopIdx)
                Client::clientCode(device);
            delete device;
        }
    }
}

struct StateAutoRunner
{
    StateAutoRunner()
    {
        std::cout << "\n--- State Pattern Example ---\n";
        State::run();
    }
};

static StateAutoRunner instance;
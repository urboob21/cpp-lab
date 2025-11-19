// Command is a behavioral design pattern that turns a request into a stand-alone object that contains all information about the request. (receivers, payloads)
// This transformation lets you pass requests as a method arguments, delay or queue a request’s execution, and support undoable operations.
// Allows an object to send a command without knowing what object will receive and handle it.
// Appicability:
// (*)   when you want to parameterize objects with operations.
// (**)  when you want to queue operations, schedule their execution, or execute them remotely.
// (***) when you want to implement reversible operations.

// UML: docs/uml/patterns_behavioral_command.drawio.svg

#include <iostream>
#include <string>

namespace
{
    namespace Command
    {

        /**
         * The Command interface usually declares just a single method for executing the command.
         * e.g. Save, Undo, Jump, Backup, CreateOrder
         */
        class ICommand
        {
        public:
            virtual void execute() const = 0;
        };

        /**
         * The Receiver class contains some business logic. Almost any object may act as a receiver.
         * Most commands only handle the details of how a request is passed to the receiver, while the receiver itself does the actual work.
         * e.g. Document, GameCharacter, DB service
         */
        class Receiver
        {
        public:
            static void doCheck()
            {
                std::cout << "Receiver checking... \n";
            };
            static void doInit()
            {
                std::cout << "Receiver initializing... \n";
            };
            static void doLaunch(const std::string &arg)
            {
                std::cout << "Receiver launching... \n\t" << arg << "\n";
            };
        };

        /**
         * Concrete Commands implement various kinds of requests.
         *  A concrete command isn’t supposed to perform the work on its own, but rather to pass the call to one of the business logic objects.
         * However, for the sake of simplifying the code, these classes can be merged.
         */
        class SimpleConcreteCommand : public ICommand
        {
        public:
            void execute() const override
            {
                std::cout << "\t SimpleCommand executed \n";
            }
        };

        class ComplexConcreteCommand : public ICommand
        {
        private:
            Receiver *m_receiver;
            std::string m_payload;

        public:
            ComplexConcreteCommand(Receiver *receiver, const std::string &payload) : m_receiver{receiver}, m_payload{payload} {};

            void execute() const override
            {
                std::cout << "\t ComplexCommand executed \n";
                this->m_receiver->doCheck();
                this->m_receiver->doInit();
                this->m_receiver->doLaunch(m_payload);
            }
        };

        /**
         *The Sender class (aka invoker) is responsible for initiating requests.
         *This class must have a field for storing a reference to a command object.
         *The sender triggers that command instead of sending the request directly to the receiver.
         *Note that the sender isn’t responsible for creating the command object. Usually, it gets a pre-created command from the client via the constructor.
         *e.g. Button, Shortcut, Scheduler, Event bus...
         */
        class Invoker
        {
        private:
            ICommand *m_on_start;
            ICommand *m_on_finish;

        public:
            explicit Invoker(ICommand *s = nullptr, ICommand *f = nullptr) : m_on_start{s}, m_on_finish{s}
            {
            }

            ~Invoker()
            {
                delete m_on_start;
                delete m_on_finish;
            }

            void setOnStart(ICommand *command)
            {
                this->m_on_start = command;
            }

            void setOnFinish(ICommand *command)
            {
                this->m_on_finish = command;
            }

            void invoke() const
            {
                if (m_on_start != nullptr)
                {
                    m_on_start->execute();
                }

                if (m_on_finish != nullptr)
                {
                    m_on_finish->execute();
                }
            }
        };

        namespace Client
        {
            void clientCode(const Invoker *invoker)
            {
                invoker->invoke();
            }

        }

        void run()
        {
            Receiver *ui = new Receiver();
            // How to execute these command when something triggered
            Invoker *invoker = new Invoker();
            invoker->setOnStart(new SimpleConcreteCommand());
            invoker->setOnFinish(new ComplexConcreteCommand(ui, "cmd --version"));
            Client::clientCode(invoker);
            delete ui;
        }
    }
}

struct CommandAutoRunner
{
    CommandAutoRunner()
    {
        std::cout << "\n--- Command Pattern Example ---\n";
        Command::run();
    }
};

static CommandAutoRunner instance;
// cppcheck-suppress-file [functionStatic]

// Facade is a structural design pattern that provides a simplified interface 
// to a library, a framework, or any other complex set of classes.
// Appicability:
// (*)   when you need to have a limited but straightforward interface to a complex subsystem.
// (**)  when you want to structure a subsystem into layers.
// UML: none

#include <iostream>
namespace
{
    namespace Problem
    {

        class AuthSubSystem
        {
        public:
            virtual ~AuthSubSystem() = default;
            void login() const
            {
                std::cout << "AuthSubSystem: login\n";
            }
        };

        class ValidatorSubSystem
        {
        public:
            virtual ~ValidatorSubSystem() = default;
            virtual void check() const
            {
                std::cout << "ValidatorSubSystem: check input\n";
            }
        };

        class LoggerSubSystem
        {
        public:
            virtual ~LoggerSubSystem() = default;
            void write() const
            {
                std::cout << "LoggerSubSystem: write log\n";
            }
        };

        class BackendSubSystem
        {
        public:
            virtual ~BackendSubSystem() = default;
            void send() const
            {
                std::cout << "BackendSubSystem: send request\n";
            }
        };

        namespace Client1
        {
            // The client must manually interact with each subsystem.
            // This creates unnecessary complexity.
            void clientCode(const ValidatorSubSystem &s1, const AuthSubSystem &s2, const LoggerSubSystem &s3, const BackendSubSystem &s4)
            {
                s1.check();
                s2.login();
                s3.write();
                s4.send();

                // In real code, the client must also
                //   - know call ordering
                //   - combine subsystem operations
                //   - manage lifecycle and error handling
            }
        }

        namespace Client2
        {
            // The client must manually interact with each subsystem.
            // This creates unnecessary complexity.
            void clientCode(const ValidatorSubSystem &s1)
            {
                s1.check();
            }
        }

        void run()
        {
            std::cout << "\n\nProblem\n";

            ValidatorSubSystem v;
            AuthSubSystem a;
            LoggerSubSystem l;
            BackendSubSystem b;

            Client1::clientCode(v, a, l, b);
            Client2::clientCode(v);
        }
    }

    namespace Facade
    {

        class AuthSubSystem
        {
        public:
            virtual ~AuthSubSystem() = default;
            void login() const
            {
                std::cout << "AuthSubSystem: login\n";
            }
        };

        class ValidatorSubSystem
        {
        public:
            virtual ~ValidatorSubSystem() = default;
            void check() const
            {
                std::cout << "ValidatorSubSystem: check input\n";
            }
        };

        class LoggerSubSystem
        {
        public:
            virtual ~LoggerSubSystem() = default;
            void write() const
            {
                std::cout << "LoggerSubSystem: write log\n";
            }
        };

        class BackendSubSystem
        {
        public:
            virtual ~BackendSubSystem() = default;
            virtual void send() const
            {
                std::cout << "BackendSubSystem: send request\n";
            }
        };

        // =======================
        // Mock subclasses (for testing)
        // =======================
        class MockBackendSubSystem : public BackendSubSystem
        {
        public:
            void send() const override
            {
                std::cout << "[MockBackend] fake-send\n";
            }
        };

        /**
         * The Facade class provides a simple interface to the complex logic of one or
         * several subsystems. The Facade delegates the client requests to the
         * appropriate objects within the subsystem. The Facade is also responsible for
         * managing their lifecycle. All of this shields the client from the undesired
         * complexity of the subsystem.
         */
        class RequestFacade
        {
        protected:
            const AuthSubSystem *auth_;
            const ValidatorSubSystem *validator_;
            const LoggerSubSystem *logger_;
            const BackendSubSystem *backend_;

            /**
             * Depending on your application's needs, you can provide the Facade with
             * existing subsystem objects or force the Facade to create them on its own.
             */
        public:
            /**
             * In this case we will delegate the memory ownership to Facade Class
             */
            explicit RequestFacade(const AuthSubSystem *s1 = nullptr,
                                   const ValidatorSubSystem *s2 = nullptr,
                                   const LoggerSubSystem *s3 = nullptr,
                                   const BackendSubSystem *s4 = nullptr)
            {
                this->auth_ = s1 ?: new AuthSubSystem;
                this->validator_ = s2 ?: new ValidatorSubSystem;
                this->logger_ = s3 ?: new LoggerSubSystem;
                this->backend_ = s4 ?: new BackendSubSystem;
            }

            ~RequestFacade()
            {
                delete auth_;
                delete validator_;
                delete logger_;
                delete backend_;
            }

            /**
             * The Facade's methods are convenient shortcuts to the sophisticated
             * functionality of the subsystems. However, clients get only to a fraction of
             * a subsystem's capabilities.
             */
            void sendRequest() const
            {
                validator_->check();
                auth_->login();
                logger_->write();
                backend_->send();
            }

            void validate() const
            {
                validator_->check();
            }
        };

        namespace Client1
        {
            /**
             * The client code works with complex subsystems through a simple interface
             * provided by the Facade. When a facade manages the lifecycle of the subsystem,
             * the client might not even know about the existence of the subsystem. This
             * approach lets you keep the complexity under control.
             */
            void clientCode(const RequestFacade &facade)
            {
                facade.sendRequest();
            }
        }

        namespace Client2
        {
            /**
             * The client code works with complex subsystems through a simple interface
             * provided by the Facade. When a facade manages the lifecycle of the subsystem,
             * the client might not even know about the existence of the subsystem. This
             * approach lets you keep the complexity under control.
             */
            void clientCode(const RequestFacade &facade)
            {
                facade.validate();
            }
        }

        void run()
        {
            std::cout << "\n\nFacade\n";
            {
                RequestFacade *facade = new RequestFacade();
                Client1::clientCode(*facade);
                Client2::clientCode(*facade);
                delete facade;
            }

            {
                // injected subsystems for mocktest
                std::cout << "\n";
                const MockBackendSubSystem *b = new MockBackendSubSystem();
                RequestFacade *facade = new RequestFacade(nullptr, nullptr, nullptr, b);
                Client1::clientCode(*facade);
                delete facade;
            }
        }
    }
}

struct FacadeAutoRuner
{
    FacadeAutoRuner()
    {
        std::cout << "\n--- Facade Pattern Example ---\n";
        Problem::run();
        Facade::run();
    }
};

static FacadeAutoRuner instance;
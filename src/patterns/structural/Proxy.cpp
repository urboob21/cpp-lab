// Proxy is a structural design pattern that lets you provide a substitute or placeholder for another object.
// A proxy controls access to the original object, allowing you to perform something
// either before or after the request gets through to the original object.
// A proxy receives client requests, does some work (access control, caching, etc.) and then passes the request to a service object.
// Appicability:
// (*)   Lazy initialization (virtual proxy) /  Logging requests (logging proxy)  / Caching request results (caching proxy)
// (**)  Access control (protection proxy). 
// (***) Local execution of a remote service (remote proxy). This is when the service object is located on a remote server.

// Composition - lazy
// Aggregation - x
// UML: docs/uml/patterns_structural_proxy.drawio.svg

#include <iostream>
namespace
{
    namespace Problem
    {
        const std::string admin = "admin";

        class IServer
        {
        public:
            virtual ~IServer() = default;
            virtual void request1() = 0;
            virtual void request2() = 0;
            virtual void request3() = 0;
        };

        class Server : public IServer
        {
        private:
            std::string _id;

        public:
            explicit Server(const std::string &id) : _id{id}
            {
                // [P1] Heavy or complex construction, so ideally should be lazy-loaded
                std::cout << "[Server] Constructor: " << _id << "\n";
            }

            // [P2] Need access control
            // [P3] Need to log requests without modifying the Server itself
            void request1() override
            {
                if (_id != admin)
                {
                    std::cout << "[Server] Invalid ID: " << _id << "\n";
                    return;
                }
                std::cout << "[Server] Handling request-1 for: " << _id << "\n";
            }

            void request2() override
            {
                if (_id != admin)
                {
                    std::cout << "[Server] Invalid ID: " << _id << "\n";
                    return;
                }
                std::cout << "[Server] Handling request-2 for: " << _id << "\n";
            }

            void request3() override
            {
                if (_id != admin)
                {
                    std::cout << "[Server] Invalid ID: " << _id << "\n";
                    return;
                }
                std::cout << "[Server] Handling request-3 for: " << _id << "\n";
            }
        };

        namespace Client
        {
            void clientCode(IServer *s)
            {
                if (s != nullptr)
                {
                    s->request1();
                    s->request2();
                    s->request3();
                }
            }
        }

        void run()
        {
            std::cout << "\n\n";

            {
                std::string connectionId = "admin";
                // [P4] The Server is constructed immediately even if we do not call any requests
                IServer *server = new Server(connectionId);
                std::cout << "User request\n";
                Client::clientCode(server);
                delete server;
            }

            {
                // [P4] Server is constructed even for invalid ID, wasting resources
                std::string invalidId = "xxx";
                Server *server = new Server(invalidId);
                Client::clientCode(server);
                delete server;
            }
        }
    }

    namespace ProxyPattern
    {
        const std::string admin = "admin";

        class IServer
        {
        public:
            virtual ~IServer() = default;
            virtual void request1() = 0;
            virtual void request2() = 0;
            virtual void request3() = 0;
        };

        class Server : public IServer
        {
        private:
            std::string _id;

        public:
            explicit Server(const std::string &id) : _id{id}
            {
                std::cout << "[Server] Constructor: " << _id << "\n";
            }

            void request1() override
            {
                std::cout << "[Server] Handling request-1 for: " << _id << "\n";
            }

            void request2() override
            {
                std::cout << "[Server] Handling request-2 for: " << _id << "\n";
            }

            void request3() override
            {
                std::cout << "[Server] Handling request-3 for: " << _id << "\n";
            }
        };

        class ServerProxy : public IServer
        {
        private:
            std::string _id;
            Server *_server;

            bool checkAccess()
            {
                std::cout << "[Proxy] Checking access before forwarding request.\n";
                if (_id != admin)
                {
                    return false;
                }

                // Lazy initialization: construct Server only on first access
                if (_server == nullptr)
                {
                    _server = new Server(_id);
                }
                return true;
            }

            void logAccess() const
            {
                std::cout << "[Proxy] Logging request time: " << _id << " .\n";
            }

        public:
            explicit ServerProxy(const std::string &id) : _id{id}, _server{nullptr}
            {
                std::cout << "[Proxy] Constructor: " << _id << "\n";
            }

            ~ServerProxy()
            {
                std::cout << "[Proxy] Destructor: " << _id << "\n";
                if (_server != nullptr)
                {
                    delete _server;
                }
            }

            void request1() override
            {
                if (checkAccess())
                {
                    _server->request1();
                    logAccess();
                }
            }

            void request2() override
            {
                if (checkAccess())
                {
                    _server->request2();
                    logAccess();
                }
            }

            void request3() override
            {
                if (checkAccess())
                {
                    _server->request3();
                    logAccess();
                }
            }
        };

        namespace Client
        {
            void clientCode(IServer *s)
            {
                if (s != nullptr)
                {
                    s->request1();
                    s->request2();
                    s->request3();
                }
            }
        }

        void run()
        {
            std::cout << "\n\n";

            {
                std::string connectionId = "admin";
                // Server is not constructed until first request is made
                IServer *server = new ServerProxy(connectionId);
                std::cout << "User request\n";
                Client::clientCode(server);
                delete server;
            }

            // Server is not constructed if id is invalid
            std::string invalidId = "xxx";
            Server *server = new Server(invalidId);
            Client::clientCode(server);
            delete server;
        }
    }

    struct ProxyAutoRunner
    {
        ProxyAutoRunner()
        {
            std::cout << "\n--- Proxy Pattern Example ---\n";
            Problem::run();
            ProxyPattern::run();
        }
    };

    static ProxyAutoRunner instance;
}

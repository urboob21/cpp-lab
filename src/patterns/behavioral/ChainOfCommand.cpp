// CoR is a behavioral design pattern that lets you pass requests along a chain of handlers.
// Upon receiving a request, each handler decides either to process the request
// or to pass it to the next handler in the chain.
// Allows an object to send a command without knowing what object will receive and handle it.
// Appicability:
// (*) when it’s essential to execute several handlers in a particular order.
// (**)when the set of handlers and their order are supposed to change at runtime.

// UML: docs/uml/patterns_behavioral_CoR.drawio.svg

#include <iostream>
#include <string>
#include <vector>

namespace
{
    namespace CoR
    {
        /*
        *  Handler - defines an interface for handling requests
        */
        class IHandler
        {
        public:
            virtual void setNextHandler(IHandler *handler) = 0;
            virtual IHandler *setNext(IHandler *handler) = 0;
            virtual void handle(const std::string &request) = 0;
        };

        class AbstractHandler : public IHandler
        {
        private:
            IHandler *m_setNext;

        public:
            AbstractHandler() : m_setNext{nullptr} {};

            void setNextHandler(IHandler *handler) override
            {
                this->m_setNext = handler;
            }

            // handler1->setNext(handler2)->setNext(handler3)
            IHandler *setNext(IHandler *handler) override
            {
                this->m_setNext = handler;
                return handler;
            }

            void handle(const std::string &request) override
            {
                if (this->m_setNext != nullptr)
                {
                    this->m_setNext->handle(request);
                }
                else
                {
                    std::cout << "\tNo handler processed request: " << request << "\n";
                }
            }
        };

        /**
         * CoreteHandlers - handles the requests it is responsible for
         * If it can handle the request it does so, otherwise it sends the request to its successor
         */
        class ConcreteHandlerGET : public AbstractHandler
        {
        private:
            static constexpr const char *header = "GET";

        public:
            void handle(const std::string &request) override
            {
                if (request.rfind(header, 0) == 0)
                {
                    // If request is eligible, handle it
                    std::cout << "\tHandle GET request: " << request << "\n";
                    // In realworld, it should be other logics here
                }
                else
                {
                    AbstractHandler::handle(request);
                }
            }
        };

        class ConcreteHandlerPUT : public AbstractHandler
        {
        private:
            static constexpr const char *header = "PUT";

        public:
            void handle(const std::string &request) override
            {
                if (request.rfind(header, 0) == 0)
                {
                    std::cout << "\tHandle PUT request: " << request << "\n";
                }
                else
                {
                    AbstractHandler::handle(request);
                }
            }
        };

        class ConcreteHandlerPOST : public AbstractHandler
        {
        private:
            static constexpr const char *header = "POST";

        public:
            void handle(const std::string &request) override
            {
                if (request.rfind(header, 0) == 0)
                {
                    std::cout << "\tHandle POST request: " << request << "\n";
                }
                else
                {
                    AbstractHandler::handle(request);
                }
            }
        };

        /**
         * Client - sends commands to the first object in the chain that may handle the command
         */
        namespace Client
        {
            void clientCode(IHandler &handler, const std::string &request)
            {
                handler.handle(request);
            }
        }

        void run()
        {
            // Setup Chain of Responsibility
            IHandler *postHandler = new ConcreteHandlerPOST();
            IHandler *gettHandler = new ConcreteHandlerGET();
            IHandler *puttHandler = new ConcreteHandlerPUT();
            postHandler->setNext(gettHandler)->setNext(puttHandler);

            // Send requests to the chain
            std::string dummy = "DUMMY ..";
            std::string postRequest = "POST /test/demo_form.php HTTP/1.1 ..";
            std::string getRequest = "GET /users/123 ..";
            std::cout << "Send dummy request\n";
            Client::clientCode(*postHandler, dummy);
            std::cout << "Send POST request\n";
            Client::clientCode(*postHandler, postRequest);
            std::cout << "Send GET request\n";
            Client::clientCode(*postHandler, getRequest);

            delete postHandler;
            delete gettHandler;
            delete puttHandler;
        }
    }

}

struct CoRAutoRunner
{
    CoRAutoRunner()
    {
        std::cout << "\n--- CoR Pattern Example ---\n";
        CoR::run();
    }
};

static CoRAutoRunner instance;
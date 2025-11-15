// Adapters is a structural design pattern that allows objects with incompatible interfaces to collaborate.
// Appicability:
// (*) When you want to use some existing class, but its interface isn’t compatible with the rest of your code
// (**)When you want to reuse several existing subclasses that lack some common functionality that can’t be added to the superclass.

// UML: docs/uml/patterns_structural_adapter.drawio.svg

#include <iostream>

namespace AdapterPattern
{
    /**
     * The Adaptee contains some useful behavior, but its interface is incompatible
     * with the existing client code. The Adaptee needs some adaptation before the
     * client code can use it.
     */
    class Adaptee
    {
    public:
        static std::string specificRequest()
        {
            return "Adaptee: The adaptee's behavior.";
        }
    };

    /**
     * The Target defines the domain-specific interface used by the client code.
     */
    class Target
    {
    public:
        virtual std::string request() const
        {
            return "Target: The target's behavior.";
        }
    };

    // ============================================================================================================
    // [Q] How can we make the clientCode works with Adaptee without change this function (e.g this is in front-end)
    // => Create an Adapter
    // ============================================================================================================

    /**
     * The Adapter makes the Adaptee's interface compatible with the Target's
     * interface.
     */
    class Adapter : public Target
    {
    private:
        Adaptee *m_adaptee;

    public:
        explicit Adapter(Adaptee *adaptee) : m_adaptee{adaptee}
        {
            std::cout << "Adapter constructer.\n";
        }

        std::string request() const override
        {
            return m_adaptee->specificRequest();
        }
    };

    /**
     * The client code supports all classes that follow the Target interface.
     */

    namespace Client
    {
        void clientCode(const Target *target)
        {
            if (target != nullptr)
                std::cout << "Output: " << target->request() << "\n";
        }
    }

    void run()
    {
        std::cout << "Client: Can work just fine with the Target objects:\n";
        Target target = Target();
        std::cout << "Target: " << target.request() << "\n";
        Client::clientCode(&target);
        std::cout << "\n\n";

        std::cout << "Client: Cannot work with the Adaptee objects:\n";
        Adaptee adaptee = Adaptee();
        std::cout << "Adaptee: " << adaptee.specificRequest() << "\n";
        // Client::clientCode(&adaptee); // error

        std::cout << "Client: But can work with it via the Adapter:\n";
        Adapter adapter = Adapter(&adaptee);
        Client::clientCode(&adapter);
        std::cout << "\n";
    }
}

namespace CaseStudy
{
    // Target interface expected by the existing system
    class PaymentSystem
    {
    public:
        virtual void payWithCard(const std::string &cardNumber)
        {
            std::cout << "Payment using card: " << cardNumber << "\n";
        }

        virtual ~PaymentSystem() = default;
    };

    // Adaptee: a new payment API with an incompatible interface
    class PayPalAPI
    {
    public:
        static void sendPayment(const std::string &email)
        {
            std::cout << "Payment sent via PayPal to " << email << "\n";
        }
    };

    // Adapter: makes PayPalAPI compatible with PaymentSystem
    class PayPalAdapter : public PaymentSystem
    {
    private:
        PayPalAPI paypal;

    public:
        void payWithCard(const std::string &cardNumber) override
        {
            // Treat the cardNumber parameter as a PayPal email
            paypal.sendPayment(cardNumber);
        }
    };

    // Client code: uses the old interface without modification
    void run()
    {
        std::string method, input;
        method = std::string("card") + std::string("");
        input = "1234-5678-9999";
        // method = std::string("paypal") + std::string("");input = "user@example.com";

        std::cout << "Choose payment method (card/paypal): " << method << "\n";

        PaymentSystem *paymentSystem = nullptr;

        if (method == "card")
        {
            paymentSystem = new PaymentSystem();
            paymentSystem->payWithCard(input);
        }
        else if (method == "paypal")
        {
            paymentSystem = new PayPalAdapter();
            paymentSystem->payWithCard(input);
        }
        else
        {
            std::cout << "Unsupported payment method!\n";
        }

        delete paymentSystem;
    }
}

struct AdapterAutoRuner
{
    AdapterAutoRuner()
    {
        std::cout << "\n--- Factory Pattern Example ---\n";
        AdapterPattern::run();
        CaseStudy::run();
    }
};

static AdapterAutoRuner instance;
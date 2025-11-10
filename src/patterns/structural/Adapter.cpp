#include <iostream>

// Adapters make legacy code work with modern classes.
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
        std::string specificRequest() const
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

struct AdapterAutoRuner
{
    AdapterAutoRuner()
    {
        std::cout << "\n--- Factory Pattern Example ---\n";
        AdapterPattern::run();
    }
};

static AdapterAutoRuner instance;

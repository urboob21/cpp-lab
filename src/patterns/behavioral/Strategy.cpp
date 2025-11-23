// cppcheck-suppress-file [functionStatic]

// Strategy is a behavioral design pattern that lets you define a family of algorithms,
// put each of them into a separate class, and make their objects interchangeable.
// Appicability:
// (*)    when you want to use different variants of an algorithm within an object and be able to switch from one algorithm to another during runtime.
// (**)   when you have a lot of similar classes that only differ in the way they execute some behavior.
// (***)  when your class has a massive conditional statement that switches between different variants of the same algorithm.
// UML: docs/uml/patterns_behavioral_strategy.drawio.svg

#include <iostream>
#include <string>
namespace
{
    namespace Strategy
    {
        class IExportStrategy
        {
        public:
            virtual ~IExportStrategy() = default;
            virtual std::string executeExportData(const std::string &content) const = 0;
        };

        class ExportContext
        {
        private:
            std::string m_content;
            IExportStrategy *m_strategy;

        public:
            ~ExportContext()
            {
                delete m_strategy;
            }

            explicit ExportContext(std::string content, IExportStrategy *const strategy = nullptr)
                : m_content{content}, m_strategy{strategy} {}

            void setExportStrategy(IExportStrategy *const strategy)
            {
                delete m_strategy;
                this->m_strategy = strategy;
            }

            // The old approach using if-else for each format is commented out:
            // if(format == "HTML") {
            //     // export HTML
            // } else if(format == "JSON") {
            //     // export JSON
            // } else if(format == "Markdown") {
            //     // export Markdown
            // }
            //
            // 1. This approach mixes data (Context) and behavior (export logic), which is hard to maintain.
            // 2. Adding new formats requires modifying this function, violating the Open/Closed Principle.
            // 3. Strategy Pattern allows each format to be a separate class, and Context only holds data.
            // ======================================================================================
            std::string exportDocument() const
            {
                if (m_strategy != nullptr)
                {
                    return this->m_strategy->executeExportData(this->m_content);
                }
                else
                {
                    std::cout << "Context: Strategy isn't set\n";
                    return "";
                }
            }
        };

        class JsonExportStrategy : public IExportStrategy
        {
        public:
            std::string executeExportData(const std::string &content) const override
            {
                return "{\"content\": \"" + content + "\" }";
            }
        };

        class HtmlExportStrategy : public IExportStrategy
        {
        public:
            std::string executeExportData(const std::string &content) const override
            {
                return "<p>" + content + "</p>";
            }
        };

        namespace Client
        {
            void clientCode(const ExportContext *ctx)
            {
                std::cout << ctx->exportDocument();
                std::cout << "\n";
            }
        }
        void run()
        {
            ExportContext *ctx = new ExportContext{"This is the report content."};
            Client::clientCode(ctx);

            std::cout << " ===HTML Export ===\n";
            ctx->setExportStrategy(new HtmlExportStrategy());
            Client::clientCode(ctx);

            std::cout << " ===JSON Export ===\n";
            ctx->setExportStrategy(new JsonExportStrategy());
            Client::clientCode(ctx);

            delete ctx;
        }
    }
}

struct StrategyAutoRunner
{
    StrategyAutoRunner()
    {
        std::cout << "\n--- Strategy Pattern Example ---\n";
        Strategy::run();
    }
};

static StrategyAutoRunner instance;
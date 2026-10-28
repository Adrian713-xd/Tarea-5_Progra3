#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include <cmath>
#include <set>



/*PREGUNTA 1
class Account {
    std::string id;
    double balance;
    std::mutex mtx;
public:
    Account(std::string id, double balance) : id(id), balance(balance) {}
    double getBalance() {
        std::lock_guard<std::mutex> lock(mtx);
        return balance;
    }
    void deposit(double amount) {
        std::lock_guard<std::mutex> lock(mtx);
        balance += amount;
    }
    void withdraw(double amount) {
        std::lock_guard<std::mutex> lock(mtx);
        balance -= amount;
    }
};

class TransactionManager {
private:
    TransactionManager() = default;
    ~TransactionManager() = default;
    TransactionManager(const TransactionManager&) = delete;
    TransactionManager& operator=(const TransactionManager&) = delete;

public:
    static TransactionManager& getInstance() {
        static TransactionManager instance;
        return instance;
    }

    void deposit(Account* account, double amount) {
        account->deposit(amount);
    }

    void withdraw(Account* account, double amount) {
        account->withdraw(amount);
    }
};

int main() {
    auto& manager = TransactionManager::getInstance();
    Account account("A1", 1000);
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&]() {
            manager.deposit(&account, 100);
            manager.withdraw(&account, 50);
        });
    }
    for (auto& t : threads) t.join();

    std::cout << "Balance final: " << account.getBalance() << std::endl;
    std::cout << "Direccion manager: " << &TransactionManager::getInstance() << std::endl;
    return 0;
}
 */

/*PREGUNTA 2

class Window { public: virtual void draw() = 0; virtual ~Window() = default; };
class Button { public: virtual void draw() = 0; virtual ~Button() = default; };
class Shader { public: virtual void compile() = 0; virtual ~Shader() = default; };

class OpenGLWindow : public Window { public: void draw() override { std::cout << "Draw OpenGL Window\n"; } };
class OpenGLButton : public Button { public: void draw() override { std::cout << "Draw OpenGL Button\n"; } };
class OpenGLShader : public Shader { public: void compile() override { std::cout << "Compile OpenGL Shader\n"; } };

class VulkanWindow : public Window { public: void draw() override { std::cout << "Draw Vulkan Window\n"; } };
class VulkanButton : public Button { public: void draw() override { std::cout << "Draw Vulkan Button\n"; } };
class VulkanShader : public Shader { public: void compile() override { std::cout << "Compile Vulkan Shader\n"; } };

class RenderFactory {
public:
    virtual std::unique_ptr<Window> createWindow() = 0;
    virtual std::unique_ptr<Button> createButton() = 0;
    virtual std::unique_ptr<Shader> createShader() = 0;
    virtual ~RenderFactory() = default;
};

class OpenGLFactory : public RenderFactory {
public:
    std::unique_ptr<Window> createWindow() override { return std::make_unique<OpenGLWindow>(); }
    std::unique_ptr<Button> createButton() override { return std::make_unique<OpenGLButton>(); }
    std::unique_ptr<Shader> createShader() override { return std::make_unique<OpenGLShader>(); }
};

class VulkanFactory : public RenderFactory {
public:
    std::unique_ptr<Window> createWindow() override { return std::make_unique<VulkanWindow>(); }
    std::unique_ptr<Button> createButton() override { return std::make_unique<VulkanButton>(); }
    std::unique_ptr<Shader> createShader() override { return std::make_unique<VulkanShader>(); }
};

int main() {
    std::unique_ptr<RenderFactory> factory = std::make_unique<OpenGLFactory>();
    auto window = factory->createWindow();
    auto button = factory->createButton();
    window->draw();
    button->draw();

    factory = std::make_unique<VulkanFactory>();
    auto shader = factory->createShader();
    shader->compile();

    return 0;
}
 */

/* PREGUNTA 3
 class Expression {
public:
    virtual double evaluate(double x) = 0;
    virtual std::shared_ptr<Expression> derivative() = 0;
    virtual std::string toString() = 0;
    virtual ~Expression() = default;
};

class Constant : public Expression {
    double value;
public:
    Constant(double v) : value(v) {}
    double evaluate(double x) override { return value; }
    std::shared_ptr<Expression> derivative() override { return std::make_shared<Constant>(0); }
    std::string toString() override { return std::to_string((int)value); }
};

class Variable : public Expression {
public:
    double evaluate(double x) override { return x; }
    std::shared_ptr<Expression> derivative() override { return std::make_shared<Constant>(1); }
    std::string toString() override { return "x"; }
};

class Add : public Expression {
    std::shared_ptr<Expression> left, right;
public:
    Add(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r) : left(l), right(r) {}
    double evaluate(double x) override { return left->evaluate(x) + right->evaluate(x); }
    std::shared_ptr<Expression> derivative() override { return std::make_shared<Add>(left->derivative(), right->derivative()); }
    std::string toString() override { return "(" + left->toString() + " + " + right->toString() + ")"; }
};

class Multiply : public Expression {
    std::shared_ptr<Expression> left, right;
public:
    Multiply(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r) : left(l), right(r) {}
    double evaluate(double x) override { return left->evaluate(x) * right->evaluate(x); }
    std::shared_ptr<Expression> derivative() override {
        return std::make_shared<Add>(
            std::make_shared<Multiply>(left->derivative(), right),
            std::make_shared<Multiply>(left, right->derivative())
        );
    }
    std::string toString() override { return "(" + left->toString() + " * " + right->toString() + ")"; }
};

class Power : public Expression {
    std::shared_ptr<Expression> base, exp;
public:
    Power(std::shared_ptr<Expression> b, std::shared_ptr<Expression> e) : base(b), exp(e) {}
    double evaluate(double x) override { return std::pow(base->evaluate(x), exp->evaluate(x)); }
    std::shared_ptr<Expression> derivative() override {
        // Implementación simplificada para x^n donde n es constante (evitando dynamic_cast)
        auto newExp = std::make_shared<Add>(exp, std::make_shared<Constant>(-1));
        auto powerPart = std::make_shared<Power>(base, newExp);
        auto derivativePart = std::make_shared<Multiply>(exp, powerPart);
        return std::make_shared<Multiply>(derivativePart, base->derivative());
    }
    std::string toString() override { return "(" + base->toString() + "^" + exp->toString() + ")"; }
};

class Sin : public Expression {
    std::shared_ptr<Expression> arg;
public:
    Sin(std::shared_ptr<Expression> a) : arg(a) {}
    double evaluate(double x) override { return std::sin(arg->evaluate(x)); }
    std::shared_ptr<Expression> derivative() override;
    std::string toString() override { return "sin(" + arg->toString() + ")"; }
};

class Cos : public Expression {
    std::shared_ptr<Expression> arg;
public:
    Cos(std::shared_ptr<Expression> a) : arg(a) {}
    double evaluate(double x) override { return std::cos(arg->evaluate(x)); }
    std::shared_ptr<Expression> derivative() override {return nullptr; }
    std::string toString() override { return "cos(" + arg->toString() + ")"; }
};

std::shared_ptr<Expression> Sin::derivative() {
    return std::make_shared<Multiply>(std::make_shared<Cos>(arg), arg->derivative());
}

int main() {
    auto x = std::make_shared<Variable>();
    auto expr = std::make_shared<Multiply>(
        std::make_shared<Add>(
            std::make_shared<Power>(x, std::make_shared<Constant>(2)),
            std::make_shared<Multiply>(std::make_shared<Constant>(3), x)
        ),
        std::make_shared<Sin>(x)
    );

    auto deriv = expr->derivative();
    std::cout << "f(x) = " << expr->toString() << std::endl;
    std::cout << "f'(x) = " << deriv->toString() << std::endl;
    std::cout << "f(2) = " << expr->evaluate(2) << std::endl;

    return 0;
}
 */

/*PREGUNTA 4
class GraphIterator {
public:
    virtual bool hasNext() = 0;
    virtual int next() = 0;
    virtual ~GraphIterator() = default;
};

class Graph {
    std::map<int, std::vector<int>> adjList;
public:
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
    }
    const std::vector<int>& getNeighbors(int u) const {
        static const std::vector<int> empty;
        auto it = adjList.find(u);
        if (it != adjList.end()) return it->second;
        return empty;
    }
};

class BFSIterator : public GraphIterator {
    const Graph& graph;
    std::queue<int> q;
    std::set<int> visited;
public:
    BFSIterator(const Graph& g, int startNode) : graph(g) {
        q.push(startNode);
        visited.insert(startNode);
    }

    bool hasNext() override {
        return !q.empty();
    }

    int next() override {
        int current = q.front();
        q.pop();

        for (int neighbor : graph.getNeighbors(current)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
        return current;
    }
};

int main() {
    Graph g;
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);

    std::unique_ptr<GraphIterator> it = std::make_unique<BFSIterator>(g, 1);
    while (it->hasNext()) {
        std::cout << it->next() << " ";
    }

    return 0;
}
*/
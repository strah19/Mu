#ifndef APPLICATION_H
#define APPLICATION_H

int main(int argc, char* argv[]);

namespace Mu {
    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        static Application& Get() { return *m_instance; }
        static Application* CreateApplication();
    private:
        static Application* m_instance;
		friend int ::main(int argc, char** argv);
    };

}

#endif // !APPLICATION_H
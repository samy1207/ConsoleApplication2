#include <iostream>
#include <ctime>
#include <cstdlib>

class GameComponent; // Forward declaration

typedef void(*FP)();

class Game {
private:
    GameComponent** components;
    int componentCount;
    FP Initialise;
    FP Terminate;

public:
    Game() : components(nullptr), componentCount(0), Initialise(nullptr), Terminate(nullptr) {}

    void Add(GameComponent* component);

    int GetComponentCount() const {
        return componentCount;
    }

    void SetInitialise(FP initFunc) {
        Initialise = initFunc;
    }

    void SetTerminate(FP termFunc) {
        Terminate = termFunc;
    }

    void Run();
};

class GameComponent {
private:
    static int instances;

protected:
    int id;

public:
    GameComponent();

    void Update(int time);
};

class DrawableGameComponent : public GameComponent {
private:
    int x;
    int y;
    std::string direction;

public:
    DrawableGameComponent();

    void Update(int time);

    void Move();

    void Draw();

    void ChangeDirection();
};

int GameComponent::instances = 0;

void InitialiseGame();

void TerminateGame();

int main() {
    srand(time(nullptr));

    Game game;
    game.SetInitialise(InitialiseGame);
    game.SetTerminate(TerminateGame);

    GameComponent* comp1 = new GameComponent();
    game.Add(comp1);

    DrawableGameComponent* comp2 = new DrawableGameComponent();
    game.Add(comp2);

    game.Run();

    // Cleanup
    delete comp1;
    delete comp2;

    return 0;
}

void Game::Add(GameComponent* component) {
    GameComponent** temp = new GameComponent * [componentCount + 1];
    for (int i = 0; i < componentCount; ++i) {
        temp[i] = components[i];
    }
    temp[componentCount++] = component;
    delete[] components;
    components = temp;
}

void Game::Run() {
    if (Initialise != nullptr) {
        Initialise();
    }

    int time = 0;
    while (time < 5) {
        for (int i = 0; i < componentCount; ++i) {
            components[i]->Update(time);
        }
        ++time;
    }

    if (Terminate != nullptr) {
        Terminate();
    }
}

GameComponent::GameComponent() {
    id = ++instances;
}

void GameComponent::Update(int time) {
    std::cout << "GameComponent " << id << " updated at time: " << time << std::endl;
}

DrawableGameComponent::DrawableGameComponent() : x(0), y(0), direction("Right") {}

void DrawableGameComponent::Update(int time) {
    GameComponent::Update(time);
    Move();
    Draw();
    ChangeDirection();
}

void DrawableGameComponent::Move() {
    if (direction == "Up") {
        y = std::max(0, y - 1);
    }
    else if (direction == "Down") {
        y = std::min(20, y + 1);
    }
    else if (direction == "Left") {
        x = std::max(0, x - 1);
    }
    else if (direction == "Right") {
        x = std::min(80, x + 1);
    }
}

void DrawableGameComponent::Draw() {
    std::cout << "DrawableGameComponent " << id << " direction: " << direction << ", x: " << x << ", y: " << y << std::endl;
}

void DrawableGameComponent::ChangeDirection() {
    std::string directions[] = { "Up", "Down", "Left", "Right" };
    std::string newDirection;
    do {
        newDirection = directions[rand() % 4];
    } while (newDirection == direction);
    direction = newDirection;
}

void InitialiseGame() {
    std::cout << "Initialising game" << std::endl;
}

void TerminateGame() {
    std::cout << "Terminating game" << std::endl;
}

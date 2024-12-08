
#define RL_LOG_LEVEL RL_LOG_NONE

#include "raylib.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip> 
#include <cmath>
#include <ctime>
#include <fstream>


// -------------------- Configurações do jogo -------------------- //
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 750;
const int GAP = 150;
const int OBSTACLE_WIDTH = 70;
const float GRAVITY_INITIAL = 0.4f; // Gravidade inicial
const float GRAVITY_INCREASE = 0.01f; // Taxa de aumento da gravidade
float GRAVITY = GRAVITY_INITIAL; // Gravidade variável
const float BIRD_JUMP = -6.0f;
const int POPULATION_SIZE = 1000;
const int MAX_GENERATIONS = 1000;

// -------------------- Variáveis para Zoom do Fundo -------------------- //
float backgroundScale = 1.0f;          // Escala inicial do fundo
float zoomSpeed = 0.07f;                // Velocidade do zoom
float zoomAmplitude = 0.05f;           // Amplitude do zoom

// -------------------- Análise do algoritimo -------------------- //
std::vector<double> fitnessHistory;

// -------------------- Estrutura da Rede Neural -------------------- //
struct NeuralNetwork {
    static const int INPUT_SIZE = 5;
    static const int H1_SIZE = 10;
    static const int H2_SIZE = 8;
    static const int OUTPUT_SIZE = 1;

    std::vector<double> weights1; 
    std::vector<double> bias1;    
    std::vector<double> weights2; 
    std::vector<double> bias2;    
    std::vector<double> weights3; 
    std::vector<double> bias3;    

    NeuralNetwork() {
        std::mt19937 gen(std::random_device{}());
        std::normal_distribution<double> dist(0.0, 1.0);

        auto he_init = [&](int fan_in) {
            double stddev = std::sqrt(2.0/fan_in);
            return dist(gen)*stddev;
        };

        weights1.resize(H1_SIZE*INPUT_SIZE);
        bias1.resize(H1_SIZE, 0.0);

        weights2.resize(H2_SIZE*H1_SIZE);
        bias2.resize(H2_SIZE, 0.0);

        weights3.resize(OUTPUT_SIZE*H2_SIZE);
        bias3.resize(OUTPUT_SIZE, 0.0);

        for (auto &w : weights1) w = he_init(INPUT_SIZE);
        for (auto &w : weights2) w = he_init(H1_SIZE);
        for (auto &w : weights3) w = he_init(H2_SIZE);
    }

    static double relu(double x) {
        return x > 0 ? x : 0;
    }

    static double sigmoid(double x) {
        return 1.0/(1.0+std::exp(-x));
    }

    std::vector<double> forward(const std::vector<double> &input) {
        std::vector<double> a1(H1_SIZE, 0.0);
        for (int i=0; i<H1_SIZE; i++){
            double sum=0;
            for (int j=0; j<INPUT_SIZE; j++)
                sum += weights1[i*INPUT_SIZE+j]*input[j];
            sum += bias1[i];
            a1[i]=relu(sum);
        }

        std::vector<double> a2(H2_SIZE,0.0);
        for (int i=0; i<H2_SIZE; i++){
            double sum=0;
            for (int j=0; j<H1_SIZE; j++)
                sum += weights2[i*H1_SIZE+j]*a1[j];
            sum += bias2[i];
            a2[i]=relu(sum);
        }

        std::vector<double> output(OUTPUT_SIZE,0.0);
        for (int i=0; i<OUTPUT_SIZE; i++){
            double sum=0;
            for (int j=0; j<H2_SIZE; j++)
                sum+=weights3[i*H2_SIZE+j]*a2[j];
            sum+=bias3[i];
            output[i]=sigmoid(sum);
        }

        return output;
    }

    std::vector<double> getWeights() const {
        std::vector<double> w;
        w.insert(w.end(), weights1.begin(), weights1.end());
        w.insert(w.end(), bias1.begin(), bias1.end());
        w.insert(w.end(), weights2.begin(), weights2.end());
        w.insert(w.end(), bias2.begin(), bias2.end());
        w.insert(w.end(), weights3.begin(), weights3.end());
        w.insert(w.end(), bias3.begin(), bias3.end());
        return w;
    }

    void setWeights(const std::vector<double> &vals) {
        int idx=0;
        for (int i=0; i<(int)weights1.size(); i++) weights1[i]=vals[idx++];
        for (int i=0; i<(int)bias1.size(); i++) bias1[i]=vals[idx++];
        for (int i=0; i<(int)weights2.size(); i++) weights2[i]=vals[idx++];
        for (int i=0; i<(int)bias2.size(); i++) bias2[i]=vals[idx++];
        for (int i=0; i<(int)weights3.size(); i++) weights3[i]=vals[idx++];
        for (int i=0; i<(int)bias3.size(); i++) bias3[i]=vals[idx++];
    }
};

// -------------------- Classe Bird -------------------- //
struct Bird {
    float x;
    float y;
    float velocity;
    int score;
    bool alive;
    double fitness;
    NeuralNetwork brain;

    Bird() {
        x=50;
        y=300;
        velocity=0;
        score=0;
        alive=true;
        fitness=0;
    }

    void update() {
        if(!alive) return;
        velocity+=GRAVITY;
        y+=velocity;
    }

    void jump() {
        velocity=BIRD_JUMP;
    }

    void draw(Texture2D &birdTex) {
        if(alive)
            DrawTexture(birdTex,(int)x,(int)y,WHITE);
    }

    std::vector<double> getInputs(float obstacleX, int obstacleHeight) {
        double ny = (double)y/SCREEN_HEIGHT;
        double nvel = (double)velocity/20.0;
        double nxDist = (double)(obstacleX - x)/SCREEN_WIDTH;
        double ntop = (double)obstacleHeight/SCREEN_HEIGHT;
        double nbottom = (double)(obstacleHeight + GAP)/SCREEN_HEIGHT;

        return {ny, nvel, nxDist, ntop, nbottom};
    }
};

// -------------------- Funções para Evolução -------------------- //
Bird crossover(const Bird &p1, const Bird &p2) {
    Bird child;
    std::vector<double> w1 = p1.brain.getWeights();
    std::vector<double> w2 = p2.brain.getWeights();
    std::vector<double> wChild(w1.size());

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> d(0.0,1.0);

    for (size_t i=0; i<w1.size(); i++){
        if(d(gen)<0.5) wChild[i]=w1[i]; else wChild[i]=w2[i];
    }

    child.brain.setWeights(wChild);
    return child;
}

void mutate(Bird &b, double mutation_rate) {
    std::vector<double> w = b.brain.getWeights();
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> d(0.0,1.0);
    std::normal_distribution<double> nd(0.0, mutation_rate);

    for (size_t i=0;i<w.size();i++){
        if(d(gen)<mutation_rate){
            w[i]+=nd(gen);
        }
    }
    b.brain.setWeights(w);
}

std::vector<Bird> evolve(std::vector<Bird> &birds, int generation, double avg_fitness) {
    // Ordenar pelo fitness
    std::sort(birds.begin(), birds.end(), [](const Bird&a, const Bird &b){
        return a.fitness>b.fitness;
    });

    double top = 0.1 - 0.004*(generation-1);
    if(top<0.01) top=0.01;
    int elite_size = std::max(1,(int)(top*birds.size()));

    double total_fitness=0;
    for (auto &b: birds) total_fitness+=b.fitness;
    if(total_fitness==0) total_fitness=1;

    std::vector<Bird> new_birds;
    // Elitismo
    for (int i=0;i<elite_size;i++){
        Bird elite=birds[i];
        new_birds.push_back(elite);
    }

    auto select_parent = [&](void)->Bird& {
        std::mt19937 gen(std::random_device{}());
        int tournament_size = (int)(0.7*birds.size());
        std::uniform_int_distribution<int> idxDist(0,(int)birds.size()-1);

        Bird *best = nullptr;
        for (int i=0;i<tournament_size;i++){
            Bird &candidate=birds[idxDist(gen)];
            if(!best || candidate.fitness>best->fitness) best=&candidate;
        }
        return *best;
    };

    double mutation_rate=0.05;

    while ((int)new_birds.size()< (int)birds.size()){
        Bird &p1=select_parent();
        Bird &p2=select_parent();
        Bird child=crossover(p1,p2);
        mutate(child,mutation_rate);
        new_birds.push_back(child);
    }

    return new_birds;
}

// -------------------- Funções de Obstáculo -------------------- //
struct Obstacle {
    float x;
    int height;
    int initialHeight;
    float heightChange;

    Obstacle(float startX) {
        x=startX;
        initialHeight=GetRandomValue(150,450);
        height=initialHeight;
        heightChange=1.0f;
    }

    void update(float obstacleSpeed) {
        x+=obstacleSpeed;
        height+=heightChange;

        const int OBSTACLE_HEIGHT_RANGE=25;
        if(height>=initialHeight+OBSTACLE_HEIGHT_RANGE || height<=initialHeight-OBSTACLE_HEIGHT_RANGE) {
            heightChange*=-1;
        }

        if (height <50) height=50;
        if(height>SCREEN_HEIGHT-GAP-50) height=SCREEN_HEIGHT-GAP-50;
    }

    void draw() {
        DrawRectangle((int)x,0,OBSTACLE_WIDTH,height,RED);
        int bottomY=height+GAP;
        int bottomH=SCREEN_HEIGHT-bottomY;
        DrawRectangle((int)x,bottomY,OBSTACLE_WIDTH,bottomH,RED);
    }

    bool offScreen() {
        return (x< -OBSTACLE_WIDTH);
    }
};

bool DetectCollision(float obstacleX, int obstacleHeight, float birdX, float birdY, float birdW, float birdH) {
    if (obstacleX >= birdX && obstacleX <= (birdX + birdW)) {
        if (birdY <= obstacleHeight || birdY + birdH >= obstacleHeight + GAP) {
            return true;
        }
    }
    return false;
}

void saveBestBird(const Bird &bestBird) {
    std::ofstream outFile("current_best_bird.txt");

    if (outFile.is_open()) {
        auto weights = bestBird.brain.getWeights();
        for (const auto &w : weights) {
            outFile << w << " ";
        }
        outFile << "\n";
        outFile.close();
    } else {
        std::cout << "Não foi possível salvar o melhor pássaro.\n";
    }
}

Bird loadBestBird() {
    Bird bestBird;

    std::ifstream inFile("utils/best_bird.txt");
    if (inFile.is_open()) {
        std::vector<double> weights;
        double weight;
        while (inFile >> weight) {
            weights.push_back(weight);
        }
        bestBird.brain.setWeights(weights);
        inFile.close();
    } else {
        std::cout << "Não foi possível carregar o melhor pássaro.\n";
    }

    return bestBird;
}

// Função para alterar a cor do texto no terminal
void setConsoleColor(int colorCode) {
    #ifdef _WIN32
        // No Windows, seria necessário usar funções do Windows API
    #else
        std::cout << "\033[" << colorCode << "m";
    #endif
}

// Função para exibir uma caixa no terminal
void printBoxedTitle(const std::string& title) {
    setConsoleColor(36); // Cor ciano para o título
    std::string border(title.size() + 4, '=');

    std::cout << "\n" << border << "\n";
    std::cout << "| " << title << " |\n";
    std::cout << border << "\n\n";
    setConsoleColor(0); // Resetar para a cor padrão
}


// Função para limpar o console
void clearConsole() {
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Linux
#endif
}

int main() {
    Bird bestBird;

    printBoxedTitle("Algoritmos Evolutivos Aplicados no Jogo RocketUp");

    std::cout << "Escolha uma opção:\n\n";
    std::cout << "[1] - Treinar novamente\n";
    std::cout << "[2] - Rodar com o melhor foguete\n\n--> ";
    int option;
    std::cin >> option;

    if (option == 2) {
        bestBird = loadBestBird();  // Carregar o melhor pássaro salvo
        std::cout << "Rodando com o melhor pássaro...\n";
    } else {
        std::cout << "Iniciando o treinamento...\n";
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird com Evolucao Genetica");
    SetTargetFPS(60);

    Texture2D birdTex = LoadTexture("imgs/foguete.png");
    Texture2D background = LoadTexture("imgs/fundo.jpeg");
    Texture2D curtains = LoadTexture("imgs/cortinas.png");

    int generation = 1;
    std::vector<Bird> birds(POPULATION_SIZE);

    bool running = true;

    clearConsole();

    while (running && generation <= MAX_GENERATIONS) {
        // Velocidade inicial a cada geração
        float initialObstacleSpeed = -6.0f;
        float obstacleSpeed = initialObstacleSpeed;

        Obstacle obstacle(SCREEN_WIDTH);

        if (option == 2) {
            birds = {bestBird};  // Rodar com o melhor pássaro
        } else {
            for (auto &b : birds) {
                b.x = 50;
                b.y = 300;
                b.velocity = 0;
                b.score = 0;
                b.alive = true;
                b.fitness = 0;
            }
        }

        bool generationRunning = true;
        int frames = 0;
        int score = 0;
        int score_prev = 0;

        while (generationRunning && running) {
            if (WindowShouldClose()) {
                running = false;
                break;
            }

            // Atualizar a escala do fundo para efeito de zoom
            backgroundScale = std::max(1.0, 1.0 + static_cast<double>(zoomAmplitude) * sin(GetTime() * zoomSpeed));

            BeginDrawing();
            ClearBackground(BLACK);
            
            // Desenhar o fundo com efeito de zoom
            DrawTextureEx(background, (Vector2){0, 0}, 0.0f, backgroundScale, WHITE);

            // A velocidade baseia-se no score. A cada 10 pontos, aumentamos a dificuldade
            // Diminuindo o valor da velocidade (mais negativo = mais rápido)
            if(score > 0 && score % 10 == 0 && score_prev + 1 == score) {
                obstacleSpeed -= 0.5f;
                score_prev = score;

                // Log para indicar o aumento da velocidade e gravidade
                setConsoleColor(36); // Cor ciano para o título
                std::cout << "\nAjustes após atingir o score " << score << ":\n";
                std::cout << "  Velocidade dos obstáculos aumentada para: " 
                        << std::fixed << std::setprecision(2) << obstacleSpeed * -1 << "\n";
                setConsoleColor(0); // Resetar para a cor padrão
            }

            obstacle.update(obstacleSpeed);

            // Se o obstáculo sair da tela, reseta e incrementa pontuação
            if(obstacle.offScreen()) {
                obstacle = Obstacle(SCREEN_WIDTH);
                score_prev = score;
                score++;
            }

            int aliveCount = 0;
            for (auto &b : birds) {
                if(!b.alive) continue;
                b.update();

                std::vector<double> input = b.getInputs(obstacle.x, obstacle.height);
                std::vector<double> output = b.brain.forward(input);

                if(output[0] > 0.5)
                    b.jump();

                // Verifica colisão ou se o pássaro saiu da tela
                if (b.y < 0 || b.y > SCREEN_HEIGHT - birdTex.height ||
                    DetectCollision(obstacle.x, obstacle.height, b.x, b.y, (float)birdTex.width, (float)birdTex.height)) {
                    b.alive = false;
                } else {
                    aliveCount++;
                    b.score++;
                    b.fitness++;
                }

                b.draw(birdTex);
            }

            if (aliveCount == 0) {
                generationRunning = false;

                if (option == 2) {
                    // Reiniciar o pássaro
                    bestBird.x = 50;
                    bestBird.y = 300;
                    bestBird.velocity = 0;
                    bestBird.score = 0;
                    bestBird.alive = true;

                    birds = {bestBird};  // Rodar apenas com o melhor pássaro

                    // Reiniciar o obstáculo
                    obstacle = Obstacle(SCREEN_WIDTH);

                    std::cout << "\nPerdeu. Reiniciando...\n";

                    // Reiniciar o loop da geração
                    generationRunning = true;
                    score = 0;
                    obstacleSpeed = initialObstacleSpeed;
                    clearConsole();
                }
            }

            if (option != 2) {
                DrawText(TextFormat("Geracao: %d", generation), 10, 10, 20, WHITE);
                DrawText(TextFormat("Vivos: %d", aliveCount), 10, 70, 20, WHITE);
            }

            DrawText(TextFormat("Score: %d", score), 10, 40, 20, WHITE);


            obstacle.draw();

            EndDrawing();

            frames++;
        }

        double max_fitness = 0;
        double total_fit = 0;
        for (auto &b : birds) {
            if(b.fitness > max_fitness)
                max_fitness = b.fitness;
            total_fit += b.fitness;
        }
        double avg_fitness = total_fit / birds.size();

        double maxFitness = 0;
        for (auto &b : birds) {
            if (b.fitness > maxFitness) {
                maxFitness = b.fitness;
                bestBird = b; 
            }
        }

        std::cout << "\n✅ Geracao " << generation << " concluida. Max fitness: " << max_fitness << " , Media: " << avg_fitness << "\n";

        if(generation >= MAX_GENERATIONS) {
            std::cout << "Numero maximo de geracoes alcancado.\n";
            break;
        }

        saveBestBird(bestBird);

        birds = evolve(birds, generation, avg_fitness);

        generation++;
    }

    UnloadTexture(birdTex);
    UnloadTexture(background);
    UnloadTexture(curtains);
    CloseWindow();
    return 0;
}
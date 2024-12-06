#include "raylib.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <ctime>

// -------------------- Configurações do jogo -------------------- //
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 750;
const int GAP = 150;
const int OBSTACLE_WIDTH = 70;
const float GRAVITY = 0.4f;
const float BIRD_JUMP = -6.0f;
const int POPULATION_SIZE = 1000;
const int MAX_GENERATIONS = 1000;

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

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird com Evolucao Genetica");
    SetTargetFPS(60);

    Texture2D birdTex = LoadTexture("imgs/uhu.png");
    Texture2D background = LoadTexture("imgs/imagem.jpeg");
    Texture2D curtains = LoadTexture("imgs/cortinas.png");

    int generation = 1;
    std::vector<Bird> birds(POPULATION_SIZE);

    bool running=true;

    while (running && generation <= MAX_GENERATIONS) {
        // Velocidade inicial a cada geração
        float initialObstacleSpeed=-6.0f;
        float obstacleSpeed=initialObstacleSpeed;

        Obstacle obstacle(SCREEN_WIDTH);

        for (auto &b: birds) {
            b.x=50;
            b.y=300;
            b.velocity=0;
            b.score=0;
            b.alive=true;
            b.fitness=0;
        }

        bool generationRunning=true;
        int frames=0;
        int score=0;
        int score_prev = 0;

        while(generationRunning && running) {
            if (WindowShouldClose()) {
                running=false;
                break;
            }

            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(background,0,0,WHITE);

            // A velocidade baseia-se no score. A cada 10 pontos, aumentamos a dificuldade
            // Diminuindo o valor da velocidade (mais negativo = mais rápido)
            if(score>0 && score%10==0 && score_prev + 1 == score) {
                obstacleSpeed -=1.0f;
                score_prev = score;
                // Após alterar, uma possibilidade é resetar o score temporário ou algo do tipo
                // Mas aqui, como o score só incrementa quando o cano sai da tela,
                // ele não vai ficar preso aumentando várias vezes no mesmo frame.
                // A cada novo cano removido e score incrementado, se atingir um novo múltiplo de 10,
                // aumentará a velocidade novamente.
            }

            obstacle.update(obstacleSpeed);

            // Se o obstáculo sair da tela, reseta e incrementa pontuação
            if(obstacle.offScreen()) {
                obstacle=Obstacle(SCREEN_WIDTH);
                score_prev = score;
                score++;
                
            }

            int aliveCount=0;
            for (auto &b: birds) {
                if(!b.alive) continue;
                b.update();

                std::vector<double> input = b.getInputs(obstacle.x, obstacle.height);
                std::vector<double> output = b.brain.forward(input);

                if(output[0]>0.5)
                    b.jump();

                // Verifica colisão ou se o pássaro saiu da tela
                if (b.y<0 || b.y>SCREEN_HEIGHT - birdTex.height ||
                    DetectCollision(obstacle.x, obstacle.height, b.x, b.y, (float)birdTex.width,(float)birdTex.height)) {
                    b.alive=false;
                } else {
                    aliveCount++;
                    b.score++;
                    b.fitness++;
                }

                b.draw(birdTex);
            }

            if (aliveCount==0) {
                generationRunning=false;
            }

            DrawText(TextFormat("Geracao: %d",generation),10,10,20,WHITE);
            DrawText(TextFormat("Score: %d",score),10,40,20,WHITE);
            DrawText(TextFormat("Vivos: %d",aliveCount),10,70,20,WHITE);

            obstacle.draw();

            EndDrawing();

            frames++;
        }

        double max_fitness=0;
        double total_fit=0;
        for (auto &b: birds) {
            if(b.fitness>max_fitness) max_fitness=b.fitness;
            total_fit+=b.fitness;
        }
        double avg_fitness = total_fit/birds.size();
        std::cout<<"Geracao "<<generation<<" concluida. Max fitness: "<<max_fitness<<" , Media: "<<avg_fitness<<"\n";

        if(generation>=MAX_GENERATIONS) {
            std::cout<<"Numero maximo de geracoes alcancado.\n";
            break;
        }

        birds = evolve(birds, generation, avg_fitness);
        generation++;
    }

    UnloadTexture(birdTex);
    UnloadTexture(background);
    UnloadTexture(curtains);
    CloseWindow();
    return 0;
}

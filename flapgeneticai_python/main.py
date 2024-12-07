import pygame
import random
import numpy as np
import queue

# Inicialização do Pygame
pygame.init()

# Dimensões da janela do jogo
SCREEN_WIDTH = 400
SCREEN_HEIGHT = 600

# Configurações do pássaro
BIRD_WIDTH = 100
BIRD_HEIGHT = 54
GRAVITY = 0.5
JUMP_STRENGTH = -7

# Configurações dos canos
PIPE_WIDTH = 50
PIPE_GAP = 150
INITIAL_PIPE_VELOCITY = -3  # Velocidade inicial a cada geração
current_pipe_velocity = INITIAL_PIPE_VELOCITY

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)

# Criar a janela do jogo
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption('Flappy Bird AI')

clock = pygame.time.Clock()

avg_fitness_prev = queue.Queue()

class NeuralNetwork:
    def __init__(self, input_size, hidden_size1, hidden_size2, output_size):
        self.weights1 = np.random.randn(hidden_size1, input_size) * np.sqrt(2 / input_size)
        self.bias1 = np.zeros(hidden_size1)
        self.weights2 = np.random.randn(hidden_size2, hidden_size1) * np.sqrt(2 / hidden_size1)
        self.bias2 = np.zeros(hidden_size2)
        self.weights3 = np.random.randn(output_size, hidden_size2) * np.sqrt(2 / hidden_size2)
        self.bias3 = np.zeros(output_size)

    def forward(self, x):
        self.z1 = np.dot(self.weights1, x) + self.bias1
        self.a1 = self.relu(self.z1)
        self.z2 = np.dot(self.weights2, self.a1) + self.bias2
        self.a2 = self.relu(self.z2)
        self.z3 = np.dot(self.weights3, self.a2) + self.bias3
        output = self.sigmoid(self.z3)
        return output

    def sigmoid(self, s):
        return 1 / (1 + np.exp(-s))

    def relu(self, s):
        return np.maximum(0, s)

    def get_weights(self):
        return np.concatenate((
            self.weights1.flatten(), self.bias1,
            self.weights2.flatten(), self.bias2,
            self.weights3.flatten(), self.bias3
        ))

    def set_weights(self, weights):
        idx = 0
        w1_size = self.weights1.size
        self.weights1 = weights[idx:idx + w1_size].reshape(self.weights1.shape)
        idx += w1_size

        b1_size = self.bias1.size
        self.bias1 = weights[idx:idx + b1_size]
        idx += b1_size

        w2_size = self.weights2.size
        self.weights2 = weights[idx:idx + w2_size].reshape(self.weights2.shape)
        idx += w2_size

        b2_size = self.bias2.size
        self.bias2 = weights[idx:idx + b2_size]
        idx += b2_size

        w3_size = self.weights3.size
        self.weights3 = weights[idx:idx + w3_size].reshape(self.weights3.shape)
        idx += w3_size

        b3_size = self.bias3.size
        self.bias3 = weights[idx:idx + b3_size]

class Bird:
    def __init__(self):
        self.x = 50
        self.y = SCREEN_HEIGHT / 2
        self.velocity = 0
        self.rect = pygame.Rect(self.x, self.y, BIRD_WIDTH, BIRD_HEIGHT)
        self.brain = NeuralNetwork(input_size=5, hidden_size1=10, hidden_size2=8, output_size=1)
        self.score = 0
        self.alive = True
        self.fitness = 0

    def update(self, pipes):
        if not self.alive:
            return

        self.velocity += GRAVITY
        self.y += self.velocity

        # Impedir o pássaro de sair da tela
        if self.y < 0:
            self.y = 0
            self.velocity = 0
        if self.y > SCREEN_HEIGHT - BIRD_HEIGHT:
            self.y = SCREEN_HEIGHT - BIRD_HEIGHT
            self.velocity = 0

        self.rect.y = self.y

        # Tomar decisão com base na visão
        inputs = self.get_inputs(pipes)
        output = self.brain.forward(inputs)

        if output[0] > 0.5:
            self.jump()

        # Atualizar a pontuação
        self.score += 1
        self.fitness += 1  # Aumenta o fitness por sobrevivência

    def get_inputs(self, pipes):
        pipe = None
        for p in pipes:
            if p.x + PIPE_WIDTH > self.x:
                pipe = p
                break
        if pipe is None:
            pipe = pipes[0]

        inputs = np.array([
            self.y / SCREEN_HEIGHT,
            self.velocity / 20,
            (pipe.x - self.x) / SCREEN_WIDTH,
            pipe.height_top / SCREEN_HEIGHT,
            (pipe.height_top + PIPE_GAP) / SCREEN_HEIGHT
        ])
        return inputs

    def jump(self):
        self.velocity = JUMP_STRENGTH

    def draw(self):
        if self.alive:
            bird_rect = bird_image.get_rect(center=(self.x + BIRD_WIDTH/2, self.y + BIRD_HEIGHT/2))
            screen.blit(bird_image, bird_rect)

class Pipe:
    def __init__(self, x):
        self.x = x
        self.height_top = random.randint(50, SCREEN_HEIGHT - PIPE_GAP - 50)
        self.height_bottom = SCREEN_HEIGHT - self.height_top - PIPE_GAP
        self.top_rect = pygame.Rect(self.x, 0, PIPE_WIDTH, self.height_top)
        self.bottom_rect = pygame.Rect(self.x, SCREEN_HEIGHT - self.height_bottom, PIPE_WIDTH, self.height_bottom)
        
        self.vertical_speed = random.choice([1, -1])
        self.vertical_change_interval = random.randint(30, 100)

    def update(self):
        global current_pipe_velocity

        # Movimento horizontal
        self.x += current_pipe_velocity
        self.top_rect.x = self.x
        self.bottom_rect.x = self.x

        # Movimento vertical dos canos, com limite de 30px
        self.height_top += self.vertical_speed
        min_top = 30
        max_top = SCREEN_HEIGHT - PIPE_GAP - 30
        if self.height_top < min_top:
            self.height_top = min_top
            self.vertical_speed = 1
        elif self.height_top > max_top:
            self.height_top = max_top
            self.vertical_speed = -1

        self.height_bottom = SCREEN_HEIGHT - self.height_top - PIPE_GAP

        self.top_rect.height = self.height_top
        self.bottom_rect.height = self.height_bottom
        self.bottom_rect.y = SCREEN_HEIGHT - self.height_bottom

        self.vertical_change_interval -= 1
        if self.vertical_change_interval <= 0:
            self.vertical_speed = random.choice([1, -1])
            self.vertical_change_interval = random.randint(30, 100)

    def draw(self):
        pygame.draw.rect(screen, RED, self.top_rect)
        pygame.draw.rect(screen, RED, self.bottom_rect)

def crossover(brain1, brain2):
    child_brain = NeuralNetwork(input_size=5, hidden_size1=10, hidden_size2=8, output_size=1)
    weights1 = brain1.get_weights()
    weights2 = brain2.get_weights()
    new_weights = np.array([weights1[i] if random.random() < 0.5 else weights2[i] for i in range(len(weights1))])
    child_brain.set_weights(new_weights)
    return child_brain

def mutate(brain, mutation_rate):
    weights = brain.get_weights()
    for i in range(len(weights)):
        if random.random() < mutation_rate:
            weights[i] += np.random.normal(0, mutation_rate)
    brain.set_weights(weights)

def evolve(birds, avg_fitness, generation):
    birds.sort(key=lambda b: b.fitness, reverse=True)
    top = 0.1 - 0.004 * (generation - 1)
    top = max(0.01, top)
    elite_size = max(1, int(top * len(birds)))
    print(f'Best fitness: {birds[0].fitness}')
    new_birds = []

    mutation_rate = 0.05

    if avg_fitness_prev.qsize() == 20:
        avg_fitness_list = list(avg_fitness_prev.queue)
        avg_of_avg = sum(avg_fitness_list[-20:]) / 20
        avg_fitness_prev.get()
        print(avg_of_avg)
        if abs(avg_of_avg - avg_fitness) < 30:
            mutation_rate = 0.7
            print('LIBERTADORES')
            avg_fitness_prev.queue.clear()
    avg_fitness_prev.put(avg_fitness)

    for i in range(elite_size):
        elite_bird = Bird()
        elite_bird.brain = birds[i].brain
        new_birds.append(elite_bird)

    total_fitness = sum(b.fitness for b in birds)
    if total_fitness == 0:
        total_fitness = 1

    def select_parent():
        tournament_size = int(0.7 * len(birds))
        tournament = random.sample(birds, tournament_size)
        tournament.sort(key=lambda b: b.fitness, reverse=True)
        return tournament[0]

    while len(new_birds) < len(birds):
        parent1 = select_parent()
        parent2 = select_parent()
        child = Bird()
        child.brain = crossover(parent1.brain, parent2.brain)
        mutate(child.brain, mutation_rate)
        new_birds.append(child)

    return new_birds

bird_image = pygame.image.load("imgs/foguete.png").convert_alpha()
background_image = pygame.image.load("imgs/fundo.jpeg").convert()

def main():
    global current_pipe_velocity

    population_size = 1000
    birds = [Bird() for _ in range(population_size)]
    generation = 1
    max_generations = 1000

    font = pygame.font.SysFont('Arial', 24)

    while True:
        # A cada nova geração, resetamos a velocidade atual
        current_pipe_velocity = INITIAL_PIPE_VELOCITY
        pipes = [Pipe(SCREEN_WIDTH + 200)]
        score = 0

        running = True
        while running:
            clock.tick(60)
            screen.blit(background_image, (0, 0))

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    return

            for pipe in pipes:
                pipe.update()
                pipe.draw()

            # Aumenta a velocidade a cada 10 pontos:
            # Quando score é múltiplo de 10 e maior que 0, aumenta a velocidade
            if score > 0 and score % 10 == 0 and score_prev + 1 == score:
                # Reduzimos mais ainda o valor negativo, aumentando a velocidade
                current_pipe_velocity -= 1
                print(f'Velocidade atual: {current_pipe_velocity}')
                score_prev = score
                # Garantimos que não fique alterando constantemente no mesmo frame
                # ao atingir um múltiplo de 10. Então, podemos, por exemplo,
                # armazenar o último checkpoint de score.
                # Mas se não precisar, ao avançar o score, não permanecerá no mesmo valor.
            
            if pipes[-1].x < SCREEN_WIDTH - 200:
                pipes.append(Pipe(SCREEN_WIDTH + 200))

            if pipes[0].x < -PIPE_WIDTH:
                pipes.pop(0)
                score_prev = score

                score += 1

            all_dead = True
            for bird in birds:
                if bird.alive:
                    bird.update(pipes)
                    bird.draw()

                    for pipe in pipes:
                        if bird.rect.colliderect(pipe.top_rect) or bird.rect.colliderect(pipe.bottom_rect):
                            bird.alive = False

                    if bird.alive:
                        all_dead = False

            score_text = font.render(f'Score: {score}', True, WHITE)
            gen_text = font.render(f'Generation: {generation}', True, WHITE)
            alive_text = font.render(f'Alive: {sum(b.alive for b in birds)}', True, WHITE)
            screen.blit(score_text, (10, 10))
            screen.blit(gen_text, (10, 40))
            screen.blit(alive_text, (10, 70))

            pygame.display.flip()

            if all_dead:
                break

        max_fitness = max(b.fitness for b in birds)
        avg_fitness = sum(b.fitness for b in birds) / len(birds)
        print(f'Geração {generation} concluída. Máximo fitness: {max_fitness}, Fitness médio: {avg_fitness}')

        if generation >= max_generations:
            print("Número máximo de gerações alcançado.")
            break

        birds = evolve(birds, avg_fitness, generation)
        generation += 1

    pygame.quit()

if __name__ == '__main__':
    main()

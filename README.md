<h1 align="center">Algoritmos Evolutivos Aplicados inspirados no Jogo Flappy Bird 🚀💻</h1>
<p align="center"> Projeto de pesquisa da disciplina SSC0713- Sistemas Evolutivos Aplicados à Robótica. </p>

<p align="center">
  <a href="#estrutura-do-projeto">Estrutura do Projeto</a> • 
  <a href="#instalacao">Instalação</a> • 
  <a href="#uso">Uso</a> • 
  <a href="#algoritmos-e-modelos">Algoritmos e Modelos</a> •
  <a href="#pontos_importantes">Pontos importantes</a> •
  <a href="#resultados">Resultados</a> •
  <a href="#contribuindo">Contribuição</a> •
  <a href="#licenca">Licença</a> •
  <a href="#agradecimentos">Agradecimentos</a> •
  <a href="#bibliografia">Bibliografia</a>
</p>

<p align="center">
   <img align="center" text-align="center" width="30%" style="margin-right:50px;" src="https://github.com/MatheusPaivaa/FlapGeneticAI/blob/main/imgs/img1.png">
   <img align="center" text-align="center" width="30%" src="https://github.com/MatheusPaivaa/FlapGeneticAI/blob/main/imgs/img2.png">
</p>

<p align="center">
Este projeto explora a aplicação de algoritmos evolutivos para treinar um agente a jogar um jogo inspirado Flappy Bird. Através da utilização de técnicas de seleção natural e mutação, o modelo é evoluído para melhorar sua performance no jogo, simulando um processo de adaptação gradual. O objetivo é observar como a evolução pode ser usada para resolver problemas de controle em ambientes dinâmicos e não determinísticos.
</p>

## <div id="estrutura-do-projeto"></div>Estrutura do Projeto
Abaixo está a estrutura de diretórios e arquivos do projeto. Cada componente foi organizado para facilitar o desenvolvimento e a manutenção das versões em Python e C++.

___

### flapgeneticai_python e flapgeneticai_cpp
1. **`main.py`** e **main.cpp**: Contém todo o código, incluindo a inicialização do Pygame, a definição do comportamento do pássaro e canos, e a implementação da rede neural e do algoritmo genético.
   
   - **NeuralNetwork**: Define a rede neural usada pelo pássaro para tomar decisões, como pular ou não. Ela tem camadas de entrada, ocultas e de saída, e usa funções de ativação como ReLU e sigmoid.
   
   - **Bird**: Controla o pássaro, definindo sua posição, velocidade e lógica de movimento (incluindo a interação com a rede neural para decidir quando pular).
   
   - **Pipe**: Representa os canos que o pássaro deve evitar. Eles se movem horizontalmente e alteram a altura vertical para aumentar a dificuldade.
   
   - **Funções de Algoritmo Genético**:
     - **Crossover**: Combina as redes neurais de dois pássaros para criar um novo pássaro.
     - **Mutate**: Aplica pequenas mudanças nas redes neurais para introduzir diversidade.
     - **Evolve**: Cria uma nova geração de pássaros, selecionando os melhores, cruzando e mutando.
   
   - **Função principal**: Controla o loop do jogo, gerencia a população de pássaros e executa o algoritmo genético a cada nova geração.

2. **`imgs/`**: Contém as imagens usadas no jogo, como a do foguete (`foguete.png`) e o fundo do jogo (`fundo.jpeg`).



## <div id="instalacao"></div>Instalação
Esta aplicação oferece versões em Python e C++. Siga os passos abaixo para configurar o ambiente e rodar a versão de sua escolha.

___

### flapgeneticai_python
### Pré-requisitos

- Python 3.7 ou superior
- PIP
- Git

### Passos

1. **Clone o repositório**:

   ```bash
   git clone https://github.com/MatheusPaivaa/FlapGeneticAI.git
   cd FlapGeneticAI
   ```

2. **Crie e ative o ambiente virtual**:

   ```bash
   python3 -m venv venv
   source venv/bin/activate  # No Windows: venv\Scripts\activate
   ```

3. **Instale as dependências**:

   ```bash
   pip install -r requirements.txt
   ```

4. **Execute o código**:

   ```bash
   python main.py
   ```

### Usando o Makefile (Opcional)

- **Instalar dependências**:

  ```bash
  make install-deps
  ```

- **Rodar o código**:

  ```bash
  make run
   ```
___

### flapgeneticai_cpp

Para rodar este projeto, é necessário ter o **Raylib** instalado no seu sistema, além de um compilador C++ com suporte a bibliotecas externas.

1. **Instalar o Raylib**

   Antes de compilar o código, você precisa instalar a biblioteca **Raylib**. Você pode seguir as instruções de instalação do Raylib para o seu sistema operacional na documentação oficial [aqui](https://terminalroot.com.br/2022/11/crie-jogos-para-windows-linux-e-web-com-raylib-c-cpp.html#google_vignette).

2. **Instalar dependências**

   No seu sistema, você pode precisar de algumas dependências adicionais. Para instalar as bibliotecas necessárias em um sistema baseado em **Linux**, use o seguinte comando:

   ```bash
   sudo apt-get update
   sudo apt-get install libx11-dev libgl1-mesa-dev libasound2-dev libpthread-stubs0-dev libudev-dev libpng-dev
   ```

3. **Compilar o projeto**

   Após a instalação do Raylib e das dependências necessárias, você pode compilar o projeto com o seguinte comando:

   ```bash
   make
   ```

4. **Executar o jogo**

   Após a compilação, você pode executar o jogo com o comando:

   ```bash
   make run
   ```

   
## <div id="uso"></div>Uso
Após a instalação, você pode começar a utilizar o projeto executando o código conforme as instruções fornecidas. Abaixo estão os detalhes sobre como interagir com a aplicação e suas funcionalidades principais.

O projeto não requer nenhuma interação com o usuário durante a execução. Ao invés disso, iremos detalhar cada componente presente no jogo:

- **Score:** Representa a pontuação no jogo. Ela é atualizada cada vez que pelo menos um indivíduo passa pelos canos com sucesso. Essa pontuação é zerada a cada nova geração para que seja possível comparar seu desempenho com relação às anteriores.

- **Generation:** É o número de gerações até o momento. Quando todos os indivíduos morrem, esse valor é atualizado e uma nova geração é criada com 1000 indivíduos novamente.

- **Alive:** É o número de indivíduos vivos até o momento.

<p align="center">
   <img align="center" text-align="center" width="15%" src="https://github.com/MatheusPaivaa/FlapGeneticAI/blob/main/imgs/score.png">
</p>


## <div id="algoritmos-e-modelos"></div>Algoritmos e Modelos

O algoritmo que utilizamos neste projeto é inspirado no processo de seleção natural, em que soluções candidatas (indivíduos) são evoluídas ao longo de várias gerações para resolver problemas complexos. Ele simula o comportamento de populações biológicas, aplicando processos como seleção, crossover(recombinação), mutação e substituição, com o objetivo de melhorar a qualidade das soluções e tentando sempre manter a diversidade genética.

### 1. Inicialização da população

- Uma população inicial é criada, onde cada indivíduo representa uma solução possível para o problema.
- Os indivíduos são representados geralmente como vetores (ou cromossomos), contendo variáveis que definem as soluções.
- Inicialmente, esses valores podem ser gerados aleatoriamente ou com base em heurísticas específicas para aumentar a qualidade inicial da população.

### 2. Avaliação (Cálculo do fitness)
- Cada indivíduo é avaliado por meio de uma função de aptidão (fitness), que atribui um valor numérico indicando a qualidade da solução que ele representa.
- Essa avaliação é específica ao problema e pode considerar múltiplos critérios, como custo, desempenho ou precisão – isso depende do problema que estamos resolvendo.
- O resultado determina a probabilidade do indivíduo ser selecionado para a próxima etapa.

### 3. Seleção
Indivíduos com maior aptidão têm maior chance de serem selecionados como "pais" para gerar a próxima geração.

- **Elitismo:** O elitismo é uma estratégia de seleção onde os melhores indivíduos de uma geração são mantidos na próxima geração sem qualquer modificação. A ideia é garantir que as melhores soluções encontradas até o momento não sejam perdidas durante o processo evolutivo. 
- **Torneio de 2:** O torneio de 2 é um método simples de seleção, onde dois indivíduos são escolhidos aleatoriamente da população e competem entre si. O vencedor do torneio, o indivíduo com maior fitness, é selecionado como um dos pais para gerar a próxima geração. 
- **Roleta:** A probabilidade de um indivíduo ser selecionado é proporcional ao seu fitness. Isso significa que indivíduos mais aptos têm uma chance maior de serem escolhidos, mas todos têm alguma chance de ser selecionados, mesmo que tenham uma aptidão mais baixa.


### 4. Crossover
- Dois ou mais indivíduos selecionados como pais combinam suas características para gerar novos indivíduos (filhos). O objetivo é explorar novas combinações de soluções com base nas melhores características dos pais.

### 5. Mutação
- Pequenas alterações aleatórias são aplicadas aos indivíduos gerados após o crossover. O objetivo é introduzir diversidade na população, ajudando a evitar que o algoritmo fique preso em ótimos locais.


### 6. Rearranjo da população (substituição)
A nova geração substitui a antiga total ou parcialmente.

- **Elitismo:** Os indivíduos mais aptos da geração anterior são preservados na nova geração.
- **Substituição completa:** Todos os indivíduos antigos são descartados e substituídos pelos novos.
- **Substituição parcial:** Apenas parte da nova geração é inserida, mantendo indivíduos da geração anterior.



## <div id="pontos_importantes"></div>Pontos importantes

### 1. Seleção dos Pais (Usando 70% da população)
- **Por que 70% e não 100%?**
  - Evita convergência prematura ao dar chance para indivíduos menos aptos contribuírem para a reprodução.
  - Preserva a diversidade genética, importante para evitar mínimos locais.
  - Foco nos melhores indivíduos, mas com margem para testar novas combinações (exploração).
  - Reduz o custo computacional, especialmente em populações grandes como a inicial de 1000 pássaros.

---

### 2. Entradas para a Rede Neural
A rede neural utiliza **5 entradas** que fornecem informações do ambiente ao pássaro:
  - Altura normalizada do pássaro (`ny`).
  - Velocidade normalizada do pássaro (`nvel`).
  - Distância horizontal normalizada ao obstáculo (`nxDist`).
  - Altura superior do obstáculo normalizada (`ntop`).
  - Altura inferior do obstáculo normalizada (`nbottom`).

Essas entradas fornecem informações suficientes para a rede neural decidir quando o pássaro deve pular, considerando a posição e o movimento relativo ao próximo obstáculo.


<p align="center">
   <img align="center" text-align="center" width="70%" src="https://github.com/MatheusPaivaa/FlapGeneticAI/blob/main/imgs/sist_evol.png">
</p>

---

### 3. Arquitetura da Rede Neural (rede neural feedforward com duas camadas ocultas):
- **Entrada (Input Layer):** 
  - Tamanho: 5 neurônios (correspondentes às 5 entradas mencionadas acima).
- **Camada Oculta 1:** 
  - 10 neurônios com ativação **ReLU**.
- **Camada Oculta 2:** 
  - 8 neurônios com ativação **ReLU**.
- **Saída (Output Layer):**
  - 1 neurônio com ativação **sigmoide**.
  - Resultado: probabilidade de o pássaro pular (threshold: 0.5).

---

### 4. Cruzamento e Mutação
- **Cruzamento:** 
  - **Crossover uniforme:** Cada peso do cérebro do filho é selecionado aleatoriamente entre os pesos dos dois pais com probabilidade de 50%.
- **Mutação:** 
  - Mutação aplicada aos pesos com uma taxa inicial de **5%**.
  - Se a evolução estagnar (baixa variação no fitness médio), a taxa de mutação aumenta para até **70%** para explorar novas soluções.

---

### 5. Mecânicas Evolutivas
- **Sobrevivência dos melhores (elitismo):**
  - O **topo da população** (os mais aptos) é preservado diretamente para a próxima geração, garantindo continuidade genética.
- **Ajustes de dificuldade:**
  - A velocidade dos canos aumenta conforme o score aumenta (a cada múltiplo de 10), simulando maior dificuldade.

---


## <div id="resultados"></div>Resultados

<p align="center">
   <img align="center" text-align="center" width="50%" src="https://github.com/MatheusPaivaa/FlapGeneticAI/blob/main/imgs/resultados.png">
</p>

1. **Max Fitness**:
   - O **Max Fitness** cresce de forma acentuada nas primeiras gerações, com um aumento considerável entre a Geração 1 (248) e a Geração 10 (3174).
   - No entanto, após a Geração 10, o crescimento do Max Fitness começa a diminuir, com flutuações nas gerações seguintes (exemplo: Geração 14 e 15 têm o mesmo valor de 5553, e a Geração 30 atinge 5748).
   - Essa tendência de crescimento mais lento nas últimas gerações sugere que, à medida que o modelo evolui e alcança melhores soluções, a busca por novos máximos se torna mais desafiadora, o que é esperado, dado que os algoritmos evolutivos podem enfrentar dificuldades em melhorar ainda mais após um certo ponto.

2. **Média**:
   - A média mostra uma tendência de aumento ao longo das gerações, mas com variações. Inicialmente, a média é baixa (Geração 1: 51.453), mas depois se estabiliza e flutua entre 1000 a 1800.
   - A Geração 25 apresenta o maior valor médio (1835.63), o que indica uma melhoria na qualidade geral das soluções ao longo das gerações.
   - Essa tendência também reflete que, enquanto os indivíduos na população estão se tornando mais aptos, a variação de fitness entre eles se reduz. 

### Conclusão:

O comportamento observado pode ser explicado por alguns fatores típicos de algoritmos evolutivos:

- **Dificuldade crescente**: à medida que o algoritmo avança, ele encontra soluções melhores, mas a busca por novas soluções ótimas se torna progressivamente mais difícil. Isso é refletido na desaceleração do crescimento do Max Fitness nas últimas gerações.
  
- **Exploração e Exploração**: O algoritmo começa explorando amplamente o espaço de soluções e, conforme avança, explora mais detalhadamente as melhores soluções encontradas, o que pode resultar em um crescimento mais gradual da fitness máxima, enquanto a média continua a melhorar.

- **Desafio de Atingir o Ótimo Global**: Como o problema se torna mais complexo com o passar das gerações, o algoritmo pode começar a se "prender" em ótimos locais ou sub-ótimos, dificultando ainda mais a melhoria contínua da fitness máxima.


## <div id="video"></div>Vídeo de apresentação
[link para o video]

## <div id="contribuindo"></div>Contribuição
Contribuições são bem-vindas! Por favor, faça um fork do repositório e envie um pull request com suas alterações.

## <div id="licenca"></div>Licença
Este projeto está licenciado sob a Licença MIT. Veja o arquivo LICENSE para mais detalhes.

## <div id="acknowledgements"></div>Agradecimentos
Gostaríamos de agradecer a Eduardo Valle Simões pela sua orientação e apoio ao longo deste projeto.

## <div id="bibliography"></div>Bibliografia
- **Evolutionary Computation: A Unified Approach**, Kenneth A. De Jong, MIT Press, Cambridge, 2006.
- **Experimental Research in Evolutionary Computation: The New Experimentalism**, Thomas Bartz-Beielstein, Springer-Verlag, London, 2006.
- **Introduction to Stochastic Search and Optimization: Estimation, Simulation, and Control**, James C. Spall, John Wiley & Sons, 2003.
- **The Design of Innovation: Lessons from and for Competent Genetic Algorithms**, David E. Goldberg, Kluwer Academic Publisher, Boston, 2002.
- **Multiobjective Optimization using Evolutionary Algorithms**, Kalyanmoy Deb, John Wiley & Sons, Chichester, UK, 2001.
- **Evolutionary Computation 1: Basic Algorithms and Operators**, T. Bäck, D. B. Fogel, Z. Michalewicz, Taylor & Francis Group, New York, 2000.
- **Evolutionary Computation 2: Advanced Algorithms and Operators**, T. Bäck, D. B. Fogel, Z. Michalewicz, Taylor & Francis Group, Institute of Physics Publishing, Bristol, UK, 2000.
- **Metaheuristics: Progress as Real Problem Solvers**, Toshihide Ibaraki, Koji Nonobe, Mutsunori Yagiura, Springer, 2005.
- **How to Solve It: Modern Heuristics**, Zbigniew Michalewicz, David B. Fogel, Springer-Verlag, Berlin, 2ª Edição, 2004.
- **Computational Principles of Mobile Robotics**, Gregory Dudek, Michael Jenkin, Cambridge Press, 2000.
- **GitLab - SSC0713: Sistemas Evolutivos Aplicados à Robótica**, disponível em: [https://gitlab.com/simoesusp/disciplinas/-/tree/master/SSC0713-Sistemas-Evolutivos-Aplicados-a-Robotica?ref_type=heads](https://gitlab.com/simoesusp/disciplinas/-/tree/master/SSC0713-Sistemas-Evolutivos-Aplicados-a-Robotica?ref_type=heads).


## Alunos
- Antonio Carlos de Almeida Micheli Neto - **14559013** ([Github](https://github.com/))
- João Pedro Viguini T.T. Correa -  **14675503** ([Github](https://github.com/jpviguini))
- Julia Cavallio Orlando - **14758721** ([Github](https://github.com/))
- Leonardo Marangoni - **14747614** ([Github](https://github.com/))
- Matheus Paiva Angarola - **12560982** ([Github](https://github.com/MatheusPaivaa))


<h1 align="center">Algoritmos Evolutivos Aplicados ao Jogo Flappy Bird 🦅💻</h1>
<p align="center"> Projeto de pesquisa da disciplina SSC0713- Sistemas Evolutivos Aplicados à Robótica, com duração de 2024. </p>

<p align="center">
  <a href="#estrutura-do-projeto">Estrutura do Projeto</a> • 
  <a href="#instalacao">Instalação</a> • 
  <a href="#uso">Uso</a> • 
  <a href="#algoritmos-e-modelos">Algoritmos e Modelos</a> •
  <a href="#resultados">Resultados</a> •
  <a href="#contribuindo">Contribuição</a> •
  <a href="#licenca">Licença</a> •
  <a href="#agradecimentos">Agradecimentos</a> •
  <a href="#bibliografia">Bibliografia</a>
</p>

<p align="center">
Este projeto explora a aplicação de algoritmos evolutivos para treinar um agente a jogar Flappy Bird. Através da utilização de técnicas de seleção natural e mutação, o modelo é evoluído para melhorar sua performance no jogo, simulando um processo de adaptação gradual. O objetivo é observar como a evolução pode ser usada para resolver problemas de controle em ambientes dinâmicos e não determinísticos.
</p>

## <div id="estrutura-do-projeto"></div>Estrutura do Projeto
...

## <div id="instalacao"></div>Instalação
...

## <div id="uso"></div>Uso
...

## <div id="algoritmos-e-modelos"></div>Algoritmos e Modelos

[breve introdução sobre o algoritmo evolutivo ....]

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



## <div id="resultados"></div>Resultados
...

## <div id="contributing"></div>Contribuição
Contribuições são bem-vindas! Por favor, faça um fork do repositório e envie um pull request com suas alterações.

## <div id="license"></div>Licença
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
- Matheus Paiva Angarola - **12560982** ([Github](https://github.com/MatheusPaivaa))
- João Pedro Viguini T.T. -  **14675503** ([Github](https://github.com/jpviguini))
- Julia Cavallio Orlando - **14758721** ([Github](https://github.com/))
- Leonardo Marangoni - **14747614** ([Github](https://github.com/))
- Antonio Carlos de Almeida Micheli Neto - **14559013** ([Github](https://github.com/))

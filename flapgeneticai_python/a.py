import matplotlib.pyplot as plt

# Dados fornecidos
geracoes = list(range(1, 31))  # Agora considerando até a geração 30
max_fitness = [
    248, 440, 1335, 3235, 4121, 4556, 4834, 4508, 6053, 3174, 4790, 5010, 4790, 5553, 
    5553, 4508, 4929, 5389, 5670, 5054, 5389, 5471, 5098, 5670, 5865, 5748, 5060, 5471, 
    5471, 5748
]
media = [
    51.453, 70.623, 63.69, 348.084, 941.194, 1191.37, 1464.63, 1356.85, 1731.65, 1320.16, 
    1198.68, 1615.86, 1569.53, 1127.34, 1329.08, 1402.01, 1199.81, 990.271, 1295.72, 1165.18, 
    1255.27, 770.75, 1185.74, 1482.96, 1835.63, 1781.39, 1787.9, 1872.15, 1256.57, 1823.33
]

# Plotando o gráfico
plt.figure(figsize=(10, 6))
plt.plot(geracoes, max_fitness, label="Max Fitness", marker='o', color='b')
plt.plot(geracoes, media, label="Média", marker='x', color='r')

# Adicionando título e rótulos
plt.title("Evolução do Max Fitness e Média por Geração")
plt.xlabel("Geração")
plt.ylabel("Valor")
plt.legend()

# Exibindo o gráfico
plt.grid(True)
plt.tight_layout()
plt.show()

# Nome do ambiente virtual
VENV_NAME = venv

# Dependências
REQUIREMENTS = requirements.txt

# Comando para criar o ambiente virtual
create-venv:
	python3 -m venv $(VENV_NAME)

# Comando para ativar o ambiente virtual (em sistemas Unix/Linux/Mac)
activate-venv:
	. $(VENV_NAME)/bin/activate

# Instalar as dependências do projeto
install-deps: create-venv
	$(VENV_NAME)/bin/pip install --upgrade pip
	$(VENV_NAME)/bin/pip install -r $(REQUIREMENTS)

# Rodar o código
run:
	$(VENV_NAME)/bin/python main.py

# Comando para rodar o código com o ambiente virtual ativado
run-venv: activate-venv run

# Limpar o ambiente virtual
clean:
	rm -rf $(VENV_NAME)

.PHONY: create-venv activate-venv install-deps run run-venv clean

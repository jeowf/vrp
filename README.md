# Vehicle Routing Problem


## Compilando

Para compilar, é necessário possuir o C++11 instalado (default na maioria dos sistemas Linux)

Em seguida, estando no diretório em que o main.cpp está, **utilize o comando**:

> g++ main.cpp -std=c++11 -o exe

## Arquivo de Configuração

Na pasta raiz do projeto há um arquivo chamado *config.txt*, conforme mostra abaixo:

> in: diretorio_casos_de_teste
> out: diretorio_dos_resultados
> opt: diretorio_das_solucoes_certas

O arquivo já está configurado, porém caso deseje alterar, basta mudar os dados nesse arquivo. Note que os casos de teste devem estar no formato do Augeret et al. (http://www.bernabe.dorronsoro.es/vrp/).

**Nota: a pasta de solução deve existir!**

## Executando

Para executar o programa, basta navegar na pasta onde o arquivo *exe* foi gerado e executar:

> ./exe

Ao executar, escolha um algoritmo para utilizar. **Por hora, há apenas o aproximativo de Clarke & Wright**, porém, a medida que formos implementado os demais algoritmos, novas opções irão surgir.

Para salvar os resultados (sobre tempo de execução de cada caso de teste), basta executar:

> ./exe > out.txt

## Comparando com a solução ótima

A solução ótima se encontra no caminho

> instances/A/solutions/

Para comparar basta verificar os arquivos com os gerados pelo nosso programa conforme o caminho especificado.

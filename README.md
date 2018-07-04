1 SIMULADOR DE CACHE

	A memória de cache é um tipo pequeno de memória de computador volátil que fornece acesso de dados de alta velocidade a um processador e
armazena programas de computador, aplicativos e dados usados ​​com freqüência. É a memória mais rápida em um computador, e normalmente é
integrado na placa-mãe e é incorporado diretamente no processador ou na memória principal de acesso aleatório (RAM). A memória de cache
fornece um armazenamento e acesso de dados mais rápidos armazenando instâncias de programas e dados acessados ​​de forma rotineira pelo
processador. Assim, quando um processador solicita dados que já possuem uma instância na memória cache, não precisa ir para a memória
principal ou o disco rígido para buscar os dados. O simulador de cache será a implementação de uma ferramenta que simula a execução de um
cache. 

1.1 DESCRIÇÃO

    Neste contexto, o projeto recebe entradas para inicialização do tamanho dos blocos/linhas em palavras, da quantidade de blocos na memória
principal e da quantidade de linhas na memória cache. O simulador receberá como entrada, em sequência, o tamanho do bloco (em número de
palavras), o número de blocos da memória principal, o número de linhas da cache, o mapeamento (1 – Direto, onde cada bloco pode ser colocado
em uma única posição na cache; 2 – Totalmente Associativo, onde cada bloco pode ser colocado em qualquer posição na cache; 3 – Parcialmente
Associativo, onde cada bloco pode ser colocado em um conjunto restrito de posições na cache.), o número de conjuntos (caso não seja
Parcialmente Associativo, ler o valor normalmente mas desconsidere-o), e por fim, a política de substituição (1 – Aleatório, onde o bloco a
substituir é escolhido aleatoriamente; 2 – FIFO, onde substitui-se o bloco mais antigo (ainda que tenha sido recentemente usado); 3 – LFU,
onde substitui-se o bloco que foi menos utilizado; 4 – LRU, onde substitui-se o bloco que não é usado há mais tempo.), através destas 
entradas o programa irá simula a execução das requisições de endereços da memória principal contidos na memória cache, fazendo a alteração da
mesma quando necessário.

1.2 FUNCIONAMENTO

    Para essa implementação, foi utilizado a linguagem C++ com a utilização de quatro bibliotecas não padrão da linguagem, sendo elas: a
string, onde os comandos "Read" e "Show" serão lidos como strings e caso seja "Read", será manipulada a string afim de encontrar o endereço
desejado pelo usuário; a vector, onde foram declarados vectors de vectors, onde cada linha se comporta como um bloco/linha de memória e os
elementos dentre deles serão as palavras contidas naquele bloco; a fstream, onde será realizada a leitura do arquivo "config.txt", que detêm
as entradas para inicialização do programa; e a ctime, onde através do clock da máquina será pego um número dentro de um intervalo de
valores, dando uma ideia de aleatoriedade, assim sendo utilizado para a política de substituição aleatório.
	Para compilação do programa via terminal, basta:
                                                usuario@usuario-pc:~ make
Para execução do programa, terá que ser alterado o arquivo "config.txt" da forma como o usuário optar, em prol da inicialização das
caracteristicas do programa, logo haverá inicialização do tamanho dos blocos/linhas em palavras, da quantidade de blocos na memória
principal e da quantidade de linhas na memória cache, onde todas as palavras contidas na cache serão inicializadas com -1, caracterizando
assim que estão vazias. Logo após, a linha a ser escrita no terminal deverá ser assim para inicialização do programa:
                                        usuario@usuario-pc:~/lab02/cache$ bin/lab02
	O programa começará sua execução, onde para a leitura terá que ser digitado "Read x", sendo x o endereço da memória que foi requerido o 
acesso de dados, caso esteja será dito HIT linha y, sendo y a linha da cache onde está o endereço da memória. Já para mostrar todos as
palavras contidas na memória cache e memória principal terá que ser digitado "Show". Por fim, enquanto haver entrada de instruções, 
continuará rodando o programa até que o usuário pressione Ctrl+D.

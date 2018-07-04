#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include "cache.h"

int main(int argc, char * argv[]) {

  /* Inicialização do arquivo de entrada com os parâmetros de configuração. */
  std::ifstream inputFile;
  inputFile.open("config.txt");

  /* Entrada dos parâmetros de configuração. */
  Memory <int> inputMemory;
  Cache <int> inputCache;
  inputFile >> inputMemory.wordsBlock >> inputCache.linesCache >>
               inputMemory.sizeMemory >> inputMemory.mapping >>
               inputMemory.sizeSets >> inputMemory.replacementPolicy;
  inputCache.wordsBlock = inputMemory.wordsBlock;

  /* Criação dos objetos baseados nos parâmetros de entrada. */
  inputMemory.createMemory(inputMemory);
  inputCache.createCache(inputCache);

  /* Inicialização do variáveis necessárias no programa. */
  std::string instruction;
  std::size_t found;
  int word = 0, blockCache = 0, blockMemory = 0, rest = 0, sizeWay = 0,
  blockWay = 0, begin = 0, end = 0, blockCacheAux = 0;
  bool state = false;

  /* Inicialização de contadores para as políticas de substituição LFU e LRU. */
  std::vector<int> blockCacheSets (inputMemory.sizeSets, 0);

  /* Dado o mapeamento entre no caso inserido pelo usuário. */
  switch (inputMemory.mapping) {
    /* DIRETO. */
    case 1:
      /* Enquanto haver entrada de instruções, continue rodando o programa ou
      o usuário pressione Ctrl+D. */
      while (std::getline (std::cin, instruction)) {
        /* Procure espaço na string instrução de entrada. */
        found = instruction.find(" ");
        /* Se encontrar, particione a string intrução em duas, sendo uma delas,
        um inteiro com o valor. */
        if (found!=std::string::npos){
          word = std::stoi(instruction.substr(found + 1));
          instruction = instruction.substr(0,found);
        }
        /* Se a função for read, execute as instruções abaixo. */
        if (instruction.compare("Read") == 0){
          blockCache = (word/inputMemory.wordsBlock)%inputCache.linesCache;
          blockMemory = word/inputMemory.wordsBlock;
          rest = word%inputMemory.wordsBlock;
          if(inputCache.elements[blockCache][rest] == word){
            std::cout << "HIT linha " << blockCache << std::endl;
          }
          else{
            std::cout << "MISS -> alocado na linha " << blockCache << "-> bloco " << blockMemory <<" substituido" << std::endl;
            changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
          }
        }
        /* Se a função for show, execute as instruções abaixo. */
        else if (instruction.compare("Show") == 0){
          inputCache.printCache(inputCache, inputMemory);
          inputMemory.printMemory(inputMemory);
        }
      }
      break;
    /* TOTALMENTE ASSOCIATIVO. */
    case 2:
      /* Sabido o mapeamento de sua política de substituição. */
      switch (inputMemory.replacementPolicy) {
        /* ALEATÓRIO. */
        case 1:
          while (std::getline (std::cin, instruction)) {
            /* Inicialização do variáveis necessárias no programa. */
            begin = 0;
            end = inputCache.elements.size();
            /* Procure espaço na string instrução de entrada. */
            found = instruction.find(" ");
            /* Se encontrar, particione a string intrução em duas, sendo uma delas,
            um inteiro com o valor. */
            if (found!=std::string::npos){
              word = std::stoi(instruction.substr(found + 1));
              instruction = instruction.substr(0,found);
            }
            blockMemory = word/inputMemory.wordsBlock;
            /* Se a função for read, execute as instruções abaixo. */
            if (instruction.compare("Read") == 0){
              /* Procurar dentro da cache. */
              state = findWord (inputCache, word, blockCache, begin, end);
              if(state == true){
                std::cout << "HIT linha " << blockCache << std::endl;
                state = false;
              }
              else{
                /* Caso não encontre, procurar um espaço vazio dentro da cache. */
                state = findWord (inputCache, -1, blockCache, begin, end);
                if(state == true){
                  changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
                }
                else{
                  /* Caso não encontre, substitua uma palavra da cache baseado
                  na política de substituição. */
                  srand (time(NULL));
                  blockCache = rand() % inputCache.linesCache;
                  changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
                }
                std::cout << "MISS -> alocado na linha " << blockCache << "-> bloco " << blockMemory <<" substituido" << std::endl;
              }
            }
            /* Se a função for show, execute as instruções abaixo. */
            else if (instruction.compare("Show") == 0){
              inputCache.printCache(inputCache, inputMemory);
              inputMemory.printMemory(inputMemory);
            }
          }
          break;
        /* FIFO. */
        case 2:
          while (std::getline (std::cin, instruction)) {
            /* Inicialização do variáveis necessárias no programa. */
            begin = 0;
            end = inputCache.elements.size();
            /* Procure espaço na string instrução de entrada. */
            found = instruction.find(" ");
            /* Se encontrar, particione a string intrução em duas, sendo uma delas,
            um inteiro com o valor. */
            if (found!=std::string::npos){
              word = std::stoi(instruction.substr(found + 1));
              instruction = instruction.substr(0,found);
            }
            blockMemory = word/inputMemory.wordsBlock;
            /* Se a função for read, execute as instruções abaixo. */
            if (instruction.compare("Read") == 0){
              /* Procurar dentro da cache. */
              state = findWord (inputCache, word, blockCache, begin, end);
              if(state == true){
                std::cout << "HIT linha " << blockCache << std::endl;
                state = false;
              }
              else{
                /* Caso não encontre, substitua uma palavra da cache baseado
                na política de substituição. */
                changeBlocks (inputMemory, inputCache, blockCacheAux%inputCache.linesCache, blockMemory);
                std::cout << "MISS -> alocado na linha " << (blockCacheAux%inputCache.linesCache) << "-> bloco " << blockMemory <<" substituido" << std::endl;
                blockCacheAux = blockCacheAux + 1;
              }
            }
            /* Se a função for show, execute as instruções abaixo. */
            else if (instruction.compare("Show") == 0){
              inputCache.printCache(inputCache, inputMemory);
              inputMemory.printMemory(inputMemory);
            }
          }
          break;
        /* LFU. */
        case 3:
          /* Inicialização de contadores para a política de substituição LFU. */
          for(int i = 0; i < inputCache.linesCache; i++) inputCache.count.push_back(0);
          while (std::getline (std::cin, instruction)) {
            /* Inicialização do variáveis necessárias no programa. */
            begin = 0;
            end = inputCache.elements.size();
            /* Procure espaço na string instrução de entrada. */
            found = instruction.find(" ");
            /* Se encontrar, particione a string intrução em duas, sendo uma delas,
            um inteiro com o valor. */
            if (found!=std::string::npos){
              word = std::stoi(instruction.substr(found + 1));
              instruction = instruction.substr(0,found);
            }
            blockMemory = word/inputMemory.wordsBlock;
            /* Se a função for read, execute as instruções abaixo. */
            if (instruction.compare("Read") == 0){
              /* Procurar dentro da cache. */
              state = findWord (inputCache, word, blockCache, begin, end);
              if(state == true){
                std::cout << "HIT linha " << blockCache << std::endl;
                inputCache.count[blockCache] = inputCache.count[blockCache] + 1;
                state = false;
              }
              else{
                state = findWord (inputCache, -1, blockCache, begin, end);
                if(state == true){
                  /* Caso não encontre, procurar um espaço vazio dentro da cache. */
                  changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
                  inputCache.count[blockCache] = 1;
                }
                else{
                  /* Caso não encontre, substitua uma palavra da cache baseado
                  na política de substituição. */
                  blockCache = inputCache.findLeastFrequentlyUsed(inputCache);
                  changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
                  inputCache.count[blockCache] = 1;
                }
                std::cout << "MISS -> alocado na linha " << blockCache << "-> bloco " << blockMemory <<" substituido" << std::endl;
              }
            }
            /* Se a função for show, execute as instruções abaixo. */
            else if (instruction.compare("Show") == 0){
              inputCache.printCache(inputCache, inputMemory);
              inputMemory.printMemory(inputMemory);
            }
          }
          break;
        /* LRU. */
        case 4:
          /* Inicialização de contadores para a política de substituição LRU. */
          for(int i = 0; i < inputCache.linesCache; i++) inputCache.count.push_back(0);
          while (std::getline (std::cin, instruction)) {
            /* Inicialização do variáveis necessárias no programa. */
            begin = 0;
            end = inputCache.elements.size();
            /* Procure espaço na string instrução de entrada. */
            found = instruction.find(" ");
            /* Se encontrar, particione a string intrução em duas, sendo uma delas,
            um inteiro com o valor. */
            if (found!=std::string::npos){
              word = std::stoi(instruction.substr(found + 1));
              instruction = instruction.substr(0,found);
            }
            blockMemory = word/inputMemory.wordsBlock;
            /* Se a função for read, execute as instruções abaixo. */
            if (instruction.compare("Read") == 0){
              /* Procurar dentro da cache. */
              state = findWord (inputCache, word, blockCache, begin, end);
              if(state == true){
                std::cout << "HIT linha " << blockCache << std::endl;
                inputCache.count[blockCache] = -1;
                state = false;
              }
              else{
                /* Caso não encontre, procurar um espaço vazio dentro da cache. */
                state = findWord (inputCache, -1, blockCache, begin, end);
                if(state == true){
                  changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
                  inputCache.count[blockCache] = -1;
                }
                else{
                  /* Caso não encontre, substitua uma palavra da cache baseado
                  na política de substituição. */
                  blockCache = inputCache.findLeastRecentlyUsed(inputCache);
                  changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
                  inputCache.count[blockCache] = -1;
                }
                std::cout << "MISS -> alocado na linha " << blockCache << "-> bloco " << blockMemory <<" substituido" << std::endl;
              }
              inputCache.changeLeastRecentlyUsed(inputCache);
            }
            /* Se a função for show, execute as instruções abaixo. */
            else if (instruction.compare("Show") == 0){
              inputCache.printCache(inputCache, inputMemory);
              inputMemory.printMemory(inputMemory);
            }
          }
          break;
      }
      break;
    /* PARCIALMENTE ASSOCIATIVO. */
    case 3:
    /* Sabido o mapeamento de sua política de substituição. */
    switch (inputMemory.replacementPolicy) {
      /* ALEATÓRIO. */
      case 1:
        while (std::getline (std::cin, instruction)) {
          /* Procure espaço na string instrução de entrada. */
          found = instruction.find(" ");
          /* Se encontrar, particione a string intrução em duas, sendo uma delas,
          um inteiro com o valor. */
          if (found!=std::string::npos){
            word = std::stoi(instruction.substr(found + 1));
            instruction = instruction.substr(0,found);
          }

          /* Inicialização de valores remetentes ao intervalo onde será operado
          no programa, dependendo do bloco. */
          blockMemory = word/inputMemory.wordsBlock;
          sizeWay = inputCache.linesCache/inputMemory.sizeSets;
          blockWay = blockMemory%inputMemory.sizeSets;
          begin = (blockWay*sizeWay);
          end = begin + sizeWay;

          /* Se a função for read, execute as instruções abaixo. */
          if (instruction.compare("Read") == 0){
            /* Procurar dentro da cache. */
            state = findWord (inputCache, word, blockCache, begin, end);
            if(state == true){
              std::cout << "HIT linha " << blockCache << std::endl;
              state = false;
            }
            else{
              /* Caso não encontre, procurar um espaço vazio dentro da cache. */
              state = findWord (inputCache, -1, blockCache, begin, end);
              if(state == true){
                changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
                state = false;
              }
              else{
                /* Caso não encontre, substitua uma palavra da cache baseado
                na política de substituição. */
                srand (time(NULL));
                blockCache = begin + rand() % sizeWay;
                changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
              }
              std::cout << "MISS -> alocado na linha " << blockCache << "-> bloco " << blockMemory <<" substituido" << std::endl;
            }
          }
          /* Se a função for show, execute as instruções abaixo. */
          else if (instruction.compare("Show") == 0){
            inputCache.printCache(inputCache, inputMemory);
            inputMemory.printMemory(inputMemory);
          }
        }
        break;
      /* FIFO. */
      case 2:
        while (std::getline (std::cin, instruction)) {
          /* Procure espaço na string instrução de entrada. */
          found = instruction.find(" ");
          /* Se encontrar, particione a string intrução em duas, sendo uma delas,
          um inteiro com o valor. */
          if (found!=std::string::npos){
            word = std::stoi(instruction.substr(found + 1));
            instruction = instruction.substr(0,found);
          }

          /* Inicialização de valores remetentes ao intervalo onde será operado
          no programa, dependendo do bloco. */
          blockMemory = word/inputMemory.wordsBlock;
          sizeWay = inputCache.linesCache/inputMemory.sizeSets;
          blockWay = blockMemory%inputMemory.sizeSets;
          begin = (blockWay*sizeWay);
          end = begin + sizeWay;

          /* Se a função for read, execute as instruções abaixo. */
          if (instruction.compare("Read") == 0){
            /* Procurar dentro da cache. */
            state = findWord (inputCache, word, blockCache, begin, end);
            if(state == true){
              std::cout << "HIT linha " << blockCache << std::endl;
              state = false;
            }
            else{
              /* Caso não encontre, substitua uma palavra da cache baseado
              na política de substituição. */
              changeBlocks (inputMemory, inputCache, (begin + blockCacheSets[blockWay]%sizeWay), blockMemory);
              std::cout << "MISS -> alocado na linha " << (begin + (blockCacheSets[blockWay])%sizeWay) << "-> bloco " << blockMemory <<" substituido" << std::endl;
              blockCacheSets[blockWay] = blockCacheSets[blockWay] + 1;
            }
          }

          /* Se a função for show, execute as instruções abaixo. */
          else if (instruction.compare("Show") == 0){
            inputCache.printCache(inputCache, inputMemory);
            inputMemory.printMemory(inputMemory);
          }
        }
        break;
      /* LFU. */
      case 3:
        /* Inicialização de contadores para a política de substituição LFU. */
        for(int i = 0; i < inputCache.linesCache; i++) inputCache.count.push_back(0);
        while (std::getline (std::cin, instruction)) {
          /* Procure espaço na string instrução de entrada. */
          found = instruction.find(" ");
          /* Se encontrar, particione a string intrução em duas, sendo uma delas,
          um inteiro com o valor. */
          if (found!=std::string::npos){
            word = std::stoi(instruction.substr(found + 1));
            instruction = instruction.substr(0,found);
          }

          /* Inicialização de valores remetentes ao intervalo onde será operado
          no programa, dependendo do bloco. */
          blockMemory = word/inputMemory.wordsBlock;
          sizeWay = inputCache.linesCache/inputMemory.sizeSets;
          blockWay = blockMemory%inputMemory.sizeSets;
          begin = (blockWay*sizeWay);
          end = begin + sizeWay;

          /* Se a função for read, execute as instruções abaixo. */
          if (instruction.compare("Read") == 0){
            /* Procurar dentro da cache. */
            state = findWord (inputCache, word, blockCache, begin, end);
            if(state == true){
              std::cout << "HIT linha " << blockCache << std::endl;
              inputCache.count[blockCache] = inputCache.count[blockCache] + 1;
              state = false;
            }
            else{
              /* Caso não encontre, procurar um espaço vazio dentro da cache. */
              state = findWord (inputCache, -1, blockCache, begin, end);
              if(state == true){
                changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
                inputCache.count[blockCache] = 1;
              }
              else{
                /* Caso não encontre, substitua uma palavra da cache baseado
                na política de substituição. */
                blockCache = inputCache.findLeastFrequentlyUsedSets(inputCache, begin, end);
                changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
                inputCache.count[blockCache] = 1;
              }
              std::cout << "MISS -> alocado na linha " << blockCache << "-> bloco " << blockMemory <<" substituido" << std::endl;
            }
          }

          /* Se a função for show, execute as instruções abaixo. */
          else if (instruction.compare("Show") == 0){
            inputCache.printCache(inputCache, inputMemory);
            inputMemory.printMemory(inputMemory);
          }
        }
        break;
      /* LRU. */
      case 4:
        /* Inicialização de contadores para a política de substituição LRU. */
        for(int i = 0; i < inputCache.linesCache; i++) inputCache.count.push_back(0);
        while (std::getline (std::cin, instruction)) {
          /* Procure espaço na string instrução de entrada. */
          found = instruction.find(" ");
          /* Se encontrar, particione a string intrução em duas, sendo uma delas,
          um inteiro com o valor. */
          if (found!=std::string::npos){
            word = std::stoi(instruction.substr(found + 1));
            instruction = instruction.substr(0,found);
          }

          /* Inicialização de valores remetentes ao intervalo onde será operado
          no programa, dependendo do bloco. */
          blockMemory = word/inputMemory.wordsBlock;
          sizeWay = inputCache.linesCache/inputMemory.sizeSets;
          blockWay = blockMemory%inputMemory.sizeSets;
          begin = (blockWay*sizeWay);
          end = begin + sizeWay;

          /* Se a função for read, execute as instruções abaixo. */
          if (instruction.compare("Read") == 0){
            /* Procurar dentro da cache. */
            state = findWord (inputCache, word, blockCache, begin, end);
            if(state == true){
              std::cout << "HIT linha " << blockCache << std::endl;
              inputCache.count[blockCache] = -1;
              state = false;
            }
            else{
              /* Caso não encontre, procurar um espaço vazio dentro da cache. */
              state = findWord (inputCache, -1, blockCache, begin, end);
              if(state == true){
                changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
                inputCache.count[blockCache] = -1;
              }
              else{
                /* Caso não encontre, substitua uma palavra da cache baseado
                na política de substituição. */
                blockCache = inputCache.findLeastRecentlyUsedSets(inputCache, begin, end);
                changeBlocks (inputMemory, inputCache, blockCache, blockMemory);
                inputCache.count[blockCache] = -1;
              }
              std::cout << "MISS -> alocado na linha " << blockCache << "-> bloco " << blockMemory <<" substituido" << std::endl;
            }
            inputCache.changeLeastRecentlyUsedSets(inputCache, begin, end);
          }

          /* Se a função for show, execute as instruções abaixo. */
          else if (instruction.compare("Show") == 0){
            inputCache.printCache(inputCache, inputMemory);
            inputMemory.printMemory(inputMemory);
          }
        }
        break;
    }
    break;
  }
  inputFile.close();
  return 0;
}

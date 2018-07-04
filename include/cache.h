#ifndef __CACHE_H__
#define __CACHE_H__

template <typename T> class Memory {
  public:
    /* Tamanho do bloco (em número de palavras). */
    T wordsBlock;
    /* Numero de blocos da memória principal. */
    T sizeMemory;
    /* Mapeamento (1 – Direto; 2 – Totalmente Associativo; 3 – Parcialmente
    Associativo). */
    T mapping;
    /* Numero de conjuntos (caso não seja Parcialmente Associativo, ler o valor
    normalmente mas desconsidere-o). */
    T sizeSets;
    /* Política de substituição (1 – Aleatório; 2 – FIFO; 3 – LFU; 4 – LRU). */
    T replacementPolicy;
    std::vector<std::vector<T>> elements;
    /* Função para criação da memória principal dada os parâmentros. */
    void createMemory (Memory <T> &inputMemory){
      std::vector <T> auxWords;
      for(int i = 0; i < (inputMemory.wordsBlock*inputMemory.sizeMemory); i++){
        for(int j = 0; j < inputMemory.wordsBlock; j++, i++) auxWords.push_back(i);
        inputMemory.elements.push_back(auxWords);
        auxWords.clear();
        i--;
      }
    }
    /* Função para "printar" a memória principal. */
    void printMemory (Memory <T> &inputMemory){
      std::cout << "MEMORIA PRINCIPAL" << std::endl;
      std::cout << "Bloco" << "\t" << "Endereço" << std::endl;
      for(size_t i = 0; i < inputMemory.elements.size(); i++){
        for(int j = 0; j < inputMemory.wordsBlock; j++)
          std::cout << i << "\t" << inputMemory.elements[i][j] << std::endl;
      }
      std::cout << std::endl;
    }
};

template <typename T> class Cache : public Memory <T> {
  public:
    /* Numero de linhas da cache */
    T linesCache;
    std::vector<std::vector<T>> elements;
    std::vector <T> count;
    /* Função para criação da memória da cache dada os parâmentros. */
    void createCache (Cache <T> &inputCache){
      std::vector <T> auxWords;
      for(int i = 0; i < (inputCache.wordsBlock*inputCache.linesCache); i++){
        for(int j = 0; j < inputCache.wordsBlock; j++, i++) auxWords.push_back(-1);
        inputCache.elements.push_back(auxWords);
        auxWords.clear();
        i--;
      }
    }
    /* Função para "printar" a memória da cache. */
    void printCache (Cache <T> &inputCache, Memory <T> &inputMemory){
      std::cout << "CACHE L1" << std::endl;
      std::cout << "Linha" << "\t" << "Bloco" << "\t" << "Endereço" << std::endl;
      for(size_t i = 0; i < inputCache.elements.size(); i++){
        for(int j = 0; j < inputCache.wordsBlock; j++)
          std::cout << i << "\t" << inputCache.elements[i][j]/inputMemory.wordsBlock << "\t" << inputCache.elements[i][j] << std::endl;
      }
      std::cout << std::endl;
    }
    /* Função para política de substituição LFU e mapeamento totalmente
    associativo. */
    T findLeastFrequentlyUsed (Cache <T> &inputCache){
      T menos = inputCache.count[0];
      T menosIndice = 0;
      for(int i = 1; i < inputCache.linesCache; i++){
        if(inputCache.count[i] < menos){
          menos = inputCache.count[i];
          menosIndice = i;
        }
      }
      return menosIndice;
    }
    /* Função para política de substituição LFU e mapeamento parcialmente
    associativo. */
    T findLeastFrequentlyUsedSets (Cache <T> &inputCache, T begin, T end){
      T menos = inputCache.count[begin];
      T menosIndice = begin;
      for(int i = begin + 1; i < end; i++) {
        if(inputCache.count[i] < menos){
          menos = inputCache.count[i];
          menosIndice = i;
        }
      }
      return menosIndice;
    }

    /* Função para política de substituição LRU e mapeamento totalmente
    associativo. */
    T findLeastRecentlyUsed(Cache <T> &inputCache){
      T maior = inputCache.count[0];
      T maiorIndice = 0;
      for(int i = 1; i < inputCache.linesCache; i++){
        if(inputCache.count[i] > maior){
          maior = inputCache.count[i];
          maiorIndice = i;
        }
      }
      return maiorIndice;
    }

    /* Função para política de substituição LRU e mapeamento parcialmente
    associativo. */
    T findLeastRecentlyUsedSets(Cache <T> &inputCache, T begin, T end){
      T maior = inputCache.count[begin];
      T maiorIndice = begin;
      for(int i = begin + 1; i < end; i++){
        if(inputCache.count[i] > maior){
          maior = inputCache.count[i];
          maiorIndice = i;
        }
      }
      return maiorIndice;
    }

    /* Função para política de substituição LRU e mapeamento totalmente
    associativo. */
    void changeLeastRecentlyUsed(Cache <T> &inputCache){
      for(int i = 0; i < inputCache.linesCache; i++){
        inputCache.count[i] = inputCache.count[i] + 1;
      }
    }

    /* Função para política de substituição LRU e mapeamento parcialmente
    associativo. */
    void changeLeastRecentlyUsedSets(Cache <T> &inputCache, T begin, T end){
      for(int i = begin + 1; i < end; i++){
        inputCache.count[i] = inputCache.count[i] + 1;
      }
    }
};

/* Função para troca de blocos entre a memória cache e a memória principal. */
template <typename T> void changeBlocks (Memory <T> &inputMemory, Cache <T> &inputCache, T blockCache, T blockMemory){
  for(T i = 0; i < inputMemory.wordsBlock; i++){
    inputCache.elements[blockCache][i] = inputMemory.elements[blockMemory][i];
  }
}

/* Função para procura de endereços dentro da memória cache. */
template <typename T> bool findWord (Cache <T> &inputCache, T word, T &block, T begin, T end){
  for(int i = begin; i < end; i++){
    for(size_t j = 0; j < inputCache.elements[i].size(); j++){
      if(inputCache.elements[i][j] == word){ block = i; return true;}
    }
  }
  return false;
}

#endif /* __CACHE_H__ */

# Notas


## 1. Identificação

- Vitor Caetano Brustolin          - 11795589
- Lucas Keiti Anbo Mihara          - 11796472
- Pedro Lucas de Moliner de Castro - 11795784


## 2. Resolução do exercício

### 2.b. Explicação do exploit

O exploit utilizado, conhecido como **Return Oriented Programming** ou **ROP**, é uma técnica derivada do ataque de **buffer overflow** que permite que um atacante execute código malicioso através do **hijack** do fluxo do código, usando das chamadas de retorno da stack e de sequências de instruções de máquina cuidadosamente escolhidas que já estão presentes na memória da máquina, chamadas de **gadgets**.

Basicamente, quando um buffer local é criado, como em `char buffer[512]`, um espaço com seu tamanho em bytes é reservado na **stack**, no entanto, algumas funções inseguras como a `gets`, utilizada nesse caso, não fazem verificação nesse tamanho ao escrever no buffer, permitindo um atacante inserir **payloads** de dados maiores que esse tamanho para sobrescrever outros dados da stack.

Isso é perigoso pois, quando uma função é chamada, a instrução **call** coloca na stack o endereço para o qual a execução deve retornar após a função chamada ser finalizada, a instrução **ret**, então, retira o esse endereço da pilha e executa um **jump** para ele. Usando o **ROP**, é possível alterar o endereço de retorno na stack e, portanto, direcionar a execução para qualquer lugar, inclusive para a região de memória do buffer onde instruções maliciosas podem ter sido inseridas pelo atacante.

No nosso caso observamos que o buffer local alocado no começo do código possui **512 bytes**, então, para implementar o código que mandaremos como payload corretamente, precisaremos sobrescrever:

```
512 bytes do buffer + 4 bytes do ebp + 4 bytes do endereço de retorno
```

Seria possível digitar o payload malicioso manualmente durante a execução do programa, mas para facilitar é possível criarmos um arquivo de payload `eg-09.in` usando o script `eg-09.sh`. O layout final do payload será:

1. Uma sequência de instruções **nop**, técnica conhecida como **nop sled**, que serve para direcionar o fluxo do código para a posição que queremos

2. O shellcode, porção pequena de código usada para injetar instruções indesejadas na execução do código original, normalmente usa-se assembly por questões de tamanho e nesse caso é o conteúdo de `eg-09.S`

3. Padding para sobrepor o **$ebp**

4. O novo endereço de retorno, que aponta para algum lugar no **nop sled**

Rodando o programa com o payload dentro do **gdb** e analisando a stack em pontos específicos da continuidade do programa, vemos que o exploit foi um sucesso e a frase _"Hacked!"_ será impressa na tela, provando que o código inserido foi executado.


### 2.c. Mecanismos de proteção

Existem diversos tipos de proteções de runtime implementadas muitas vezes pelo próprio compilador, como é o caso do **gcc** e do **clang**, para garantir a segurança contra esses tipos de ataques, dentre elas incluem:

- O **Address space layout randomization** ou **ASLR**, que, para evitar que um invasor pule de forma simples para uma função explorada na memória, organiza aleatoriamente as posições do espaço de endereço das principais áreas de dados de um processo, incluindo a base do executável e as posições da pilha, heap e as bibliotecas carregadas

- O **Stack Canary protection**, que funciona colocando um pequeno inteiro, cujo valor é escolhido aleatoriamente no início do programa, logo antes do ponteiro de retorno da pilha que será verificado antes de saltar para o ponteiro. Boa parte dos exploits de buffer overflow sobrescrevem a memória dos endereços mais baixos para os mais altos, fazendo com que, para sobrescrever o ponteiro de retorno, o canary também seja sobrescrito, invalidando o pulo

Para que o exploit desenvolvido funcionasse, foi necessário compilar o programa alvo com flags como `-fno-stack-protector` para desativar os **stack canaries** e usar o debugger **gdb** onde o **ASLR** é desativado por padrão.

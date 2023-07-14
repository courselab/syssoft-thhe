# Notas


## 1. Identificação

- Vitor Caetano Brustolin          - 11795589
- Lucas Keiti Anbo Mihara          - 11796472
- Pedro Lucas de Moliner de Castro - 11795784



## 2. Processo de resolução

### 2.1. Análise inicial

Primeiramente, ao tentar executar o binário `encrypt` nota-se que isso não é possível devido a ausência da biblioteca compartilhada `libcypher.so`. Usando `readelf -d encrypt` é possível verificar que o programa usa as bibliotecas `libcypher.so` e `libc.so.6`. Sua execução, porém, não será necessária para a resolução.

Executando o `docrypt` com um arquivo de argumento, recebemos um prompt em texto solicitando usuário e senha com as mensagens _"User name:"_ e _"Authorization key:"_, caso sejam fornecidas credenciais incorretas a mensagem _"Access denied."_ é mostrada e o programa se encerra, mas usando a o usuário **foo** e a senha **yoda** fornecidos na descrição do trabalho, um terceiro prompt _"Decryption key:"_ é solicitado.

Percebe-se que esse algoritmo de criptografia utiliza chaves simétricas, pois, usando a chave **test** usada para encriptar o arquivo `test.cry` sua decriptação é feita corretamente para _"Hello World"_ e qualquer outra chave produz lixo. Não se sabe, no entanto, qual chave foi utilizada para encriptar `sample.cry`.

Finalmente, é importante notar que não é possível alterar os arquivos `docrypt` e `libauth.so` já que passam por uma checagem de hash simulada pelo script `check.sh`, alterá-los de qualquer forma faz com que tal checagem falhe e a invasão seria comprometida.


### 2.2. Quebra da autenticação de usuário

Como o código fonte de `docrypt` não foi fornecido, para analisá-lo será necessário fazer seu disassemble e verificar as instruções para a máquina, para isso é possível usar o comando `objdump -d docrypt`. Sabendo que o programa faz prompts de texto para ler cada informação, é possível procurar por comandos **call** para funções da libc de escrita e leitura do terminal.

De fato, existe uma parcela com chamadas em ordem para **fwrite**, **fgets**, **fwrite**, **fgets** e uma função chamada **authorize** que provavelmente representam o prompt do usuário, o da senha e a verificação desses dados. O valor de retorno de **authorize**, colocado no registrador **%eax**, é testado e, se é 0, há um pulo para as instruções de notificação do erro, ou seja, a função retorna um booleano, sendo 0 para falha e outro valor para sucesso.

Procurando pela definição de **authorize**, não é possível encontrá-la no próprio binário, mas utilizando `readelf -d docrypt` verifica-se que o programa usa as biblioteca dinâmicas `libauth.so` e `libc.so.6`. Fazendo o disassemble de `libauth.so` e procurando pelo nome da função encontra-se sua definição, isso significa que o link para tal chamada é feito dinamicamente.

As verificações de hash citadas anteriormente impedem a remoção da chamada da função **authorize** em `docrypt` ou a mudança de seu comportamento em `libauth.so`, mas como o link é feito dinamicamente e o nome da função é conhecido, é possível criar um outro programa que defina uma função com mesmo nome que sempre retorne um valor verdadeiro e fazer com que esta seja carregada antes que a função original, a substituindo.

Dessa forma, foi criado o arquivo [`hack.c`](hack.c) com essa função que será compilado na biblioteca dinâmica `libhack.so`. Vale ressaltar que o binário `docrypt` está no formato **elf32** e, por isso, a biblioteca deve ser compilada também nesse formato usando a flag `-m32`, o que exige a instalação de **gcc-multilib** caso a arquitetura local seja de 64 bits.

Para garantir que tal código seja carregado primeiro, é necessário acrescentar nossa biblioteca à variável de ambiente **LD_PRELOAD** antes de executar o `docrypt`. Todo esse processo foi automatizado no `Makefile`, permitindo utilizar o comando `make` para compilar os arquivos e `make run` para executá-lo com o exploit. Opcionalmente é possível alterar os parâmetros de execução **FILE** e **KEY** como no exemplo `make run FILE=test.cry KEY=test`.

Com isso, a autenticação de usuários foi quebrada sem alterar os arquivos com verificação de hash, o que pode ser checado ao executar `./check.sh`. Colocando qualquer valor para usuário e senha, a decriptação de qualquer arquivo será bem sucedida desde que sua chave de encriptação seja conhecida.


### 2.3. Identificação da chave criptográfica

Para encontrar a chave de encriptação, voltamos nossa atenção ao binário `encrypt`, pois é possível que a chave utilizada seja um valor estático fixo que tenha sido colocado em uma constante diretamente no código. Se esse é o caso, é possível encontrar essa informação na seção de dados do binário.

Fazendo o disassemble com `objdump -D encrypt`, nota-se que na seção **.rodata** existe uma variável chamada **encrypt_key** cujo valor dos bytes em hexadecimal é **65 61 73 79**, fazendo a conversão desses bytes para caracteres ascii obtemos a string **"easy"**.


### 2.4. Decriptação do sample

Utilizando o `Makefile` com o exploit de autenticação de usuários para executar o programa `docrypt` no arquivo `sample.cry` com a chave **easy** encontrada na análise do binário `encrypt` descobre-se que seu conteúdo é:

```
Excerpt of RFC-1392 (Request for Comments)
-------------------------------------------------------------

Network Working Group                           G. Malkin
Request for Comments: 1392                      Xylogics, Inc.
FYI: 18                                         T. LaQuey Parker
                                                UTexas
                                                Editors
                                                January 1993


                               (...)

                     Internet Users' Glossary


hacker
   A person who delights in having an intimate understanding of the
   internal workings of a system, computers and computer networks in
   particular.  The term is often misused in a pejorative context,
   where "cracker" would be the correct term.  See also: cracker.
```



## 3. Conselhos de segurança

A primeira dica para melhorar a segurança da empresa é não distribuir binários com símbolos de debugging, ou seja, compilados com **-g**. Essas informações facilitam a análise dos dados por parte de um hacker e, nesse caso, nos permitiram descobrir o nome da função **authorize** e identificar a variável global **encrypt_key**.

A utilização de bibliotecas dinâmicas para execução de parcelas críticas do código também é um risco, pois permitem o **Dynamic Linker Hijacking** exemplificado nesse exercício. O uso de bibliotecas estáticas é uma alternativa possível, mas se for inviável, analisar a variável local **LD_PRELOAD** e o arquivo `/etc/ld.so.preload` são passos necessários na execução dos processos para evitar esses ataques.

Por fim, o armazenamento da chave de encriptação direto no código é uma má prática já que essa pode ser facilmente encontrada no binário. Para salvar chaves fixas seria melhor o uso de variáveis de ambiente, mas uma opção melhor seria a utilização de chaves dinâmicas definidas em um arquivo de configuração ou passadas via entrada padrão.

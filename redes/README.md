# Trabalho 3: Servidor de Arquivos

### Implementação de um servidor de arquivos utilizando a API de _Sockets_ na linguagem C.

Os requisitos do servidor são: 
* Múltiplos clientes podem estar conectados e serem atendidos simultaneamente no servidor.
* O servidor deve responder aos comandos dos clientes, apresentando aos mesmos, como parte da
resposta, o código/status da execução do comando requisitado.
* O servidor deve executar os seguintes comandos dos clientes:
  - ```GET arquivo```
    cliente obtém _arquivo_ existente no servidor.
  - ```CREATE arquivo```
    cliente cria _arquivo_ no servidor.
  - ```REMOVE arquivo```
    cliente deleta _arquivo_ existente no servidor.
  - ```APPEND “conteúdo” arquivo```
    cliente anexa o _conteúdo_ entre aspas em _arquivo_ existente.

A implementação foi testada utilizando clientes genéricos via _telnet_.

### Compilando e executando:

Abra um terminal e digite:
```
gcc -Wall -pthread -o file_server file_server.c
./file_server <port>
```
Os comandos acima farão com que o servidor (_ip: 127.0.0.1_) seja executado e passe a ouvir requisições na porta indicada em <_port_>.

Abra outro(s) terminal(is) e digite:
```
telnet 127.0.0.1 <port>
```
Assim, será inicializada a comunicação de um cliente com o servidor ouvindo em <_port_>.

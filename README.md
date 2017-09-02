# Projeto-MAC216

Boa tarde, senhores. Vamos destruir neste projeto!
E para isso, utilizaremos o jovem e conhecido, GitHubus (GitHub
pros mais íntimos).

Bom, vamos começar esse mini-tutorial:

1) Vou deixar de referência esse cheatsheet para Git, com vários comandos.
Link: https://www.git-tower.com/blog/content/posts/54-git-cheat-sheet/git-cheat-sheet-large01.png

2) Apesar desses vários comandos, o git usa apenas 5 comandos básicos:
git add <nomedoarquivo> ou git add . (".", "*", servem para adicionar todas as mudanças)
git commit -m "mensagem"
git push
git pull
git status

3) O git é algo bem simples. Você adiciona mudanças nos arquivos do seu repositório.
Eles são atualizados. E todos com o repositório tem seus arquivos atualizados (quando
realizam o git pull).

4) Mas como ele funciona?
Bom, os passos para utilizá-lo são simples. Entre no seu terminal.
Na pasta em que deseja fazer a "copia" (clone) do repositório do GitHub (repositório
é um nome chique para dizer: pasta que tem git e as pessoas acessam), digite:
git clone url

Você obtém a url entrando na página do GitHub. Vai aparecer em um botão: Clone or Download,
no nosso caso, já enviei o link.

5) Agora que ele está clonado, basta utilizar os comandos citados em (2). E como eles funcionam?
Para sabermos como anda a situação dos arquivos do seu repositório (dentro do seu PC), você digita:
git status
E o git mostra o que foi alterado.
Bom, dado que já nos certificamos dessas mudanças, você diz pro git: "olha, esses aqui você vai guardar,
pois vou enviá-los". Em gitês:
git add nomedoarquivo OU git add *
Pronto, eles foram adicionados. Mas calma lá, jovem. Você precisa commitar. Sim, commitar. Isso diz pro
git que você se certificou que quer enviá-los para o repo que todos tem acesso, não só o seu (cópia
local). Digitando:
git commit -m "mensagem"
Você realiza essa tarefa. DICA: mensagens são meio que obrigatórias, deixe elas sucintas e informativas.
Commitei, pronto. NÃÃÃÃÕ, jovem. Você tem que dar um push, para o git, de fato, realizar suas mudanças a todos.
Digite:
git push
Ele vai pedir seu usuário e senha. Isso, para acessar seu repo do GitHub.

Agora sim, concluimos o envio de uma alteração. Para modificar as nossas cópias locais, com as alterações 
dos outros que acessam o repositório, digitamos:
git pull

Se tudo ocorrer bem, tudo ocorreu bem. Caso contrário, deu errado. Conflitos de mudanças nos arquivos ocorrem,
basta dar uma checada no cheatsheet, que tem os comandos lá.

ABraçus,

shadowtuts
# -*- coding: utf-8 -*-
import pygame as pg
import fileinput
import sys
from math import *

# inicialização do sistema
pg.init()

# altura e largura da arena, em células
H, W = 15,15

# largura da célula
l = 30
# altura da célula
L = 2*l/sqrt(3)

# cria a janela
scr = pg.display.set_mode((2*W*l+l,int(1.5*L*H+L/2)))
# só para simplificar, atualização da imagem
atu = pg.display.update


# pinta o fundo da janela
scr.fill((0,0,0))
atu()

def convert(i ,j):
    """
    Converte a coordenada (i,j) para a posição em pixels na janela
    """
    x = (1 + i%2)*l + 2*j*l
    y = 1.5*i*L + L
    return x,y

class Robo (pg.sprite.Sprite):
    """
    Representa um Robô, possui uma imagem
    """
    def __init__(s, img):
        """
        Construtor, img é o nome do arquivo com a imagem
        """
        pg.sprite.Sprite.__init__(s)
        s.image = pg.image.load(img)
        s.rect = s.image.get_rect()

    def draw(s, i,j):
        """
        Desenha o robô na célula (i,j)
        """
        x, y = convert(i,j)
        # fronteiras do sprite
        l0,h0,l1,h1 = s.rect
        print '(' + str(i) + ',' + str(j) + ')'
        # corrige a posição de acordo com o tamanho do sprite
        s.rect.topleft = [x-l1/2,y-h1/2]
        # desenha
        scr.blit(s.image, s.rect)


class cell:
    """
    Representa uma das células da arena
    """

    # a função recebe (i,j), t (tipo de terreno) e uma img opcional
    def __init__(s,i,j,t,img):
        """
        Construtor, i e j formam a coordenada
        """
        # Posição do centro
        s.cx, s.cy = convert(i,j)
        # Cor
        #Terreno: GRAMA
        if t == 0:
            s.cor = (71,193,73)
        #Terreno: AREIA
        elif t == 1:
            s.cor = (219, 216,129)
        #Terreno: PEDRA
        elif t == 2:
            s.cor = (127, 127, 123)
        #Terreno: GELO
        elif t == 3:
            s.cor = (255, 255, 249)
        #Terreno; ÁGUA
        elif t == 4:
            s.cor = (155, 187, 221)
        elif t == 5:
            s.cor = (191, 171, 219)
        else:
            s.cor = (190,190,190)
            s.image = pg.image.load(img)
            s.rect = s.image.get_rect()



        # Vértices do hexágono
        s.pontos = (
            (s.cx,   s.cy-L),
            (s.cx+l, s.cy-L/2),
            (s.cx+l, s.cy+L/2),
            (s.cx,   s.cy+L),
            (s.cx-l, s.cy+L/2),
            (s.cx-l, s.cy-L/2),
            )
    def draw(s) :
        """
        Desenha a célula
        """
        pg.draw.polygon(scr, s.cor, s.pontos, 0)
        pg.draw.lines(scr, (0,0,0), True, s.pontos, 2)
        if t == 6:
          x, y = convert(i,j)
          # fronteiras do sprite
          l0,h0,l1,h1 = s.rect
          # corrige a posição de acordo com o tamanho do sprite
          s.rect.topleft = [x-l1/2,y-h1/2]
          # desenha a bandeira da base na célula
          scr.blit(s.image, s.rect)


# Construção da matriz que representa a arena
arena = []
"""
#abrir o arquivo arena.txt no modo leitura
ap = open(sys.argv[1], 'r');

for i in range(W):

    for j in range(H):
        item = ap.readline().split()
        #print 'item: ' + str(item)
        t = item[3]
        arena[i].append(cell(i,j,t))
        arena[i][j].draw()
"""
# lista dos robôs definidos
robs = []
basecount = -1
# Leitura dos comandos pela entrada padrão ou de um arquivo
for line in fileinput.input():
    r = line.split()
    # linha começando com 'fim', termina o programa
    if r[0] == 'fim':
        break

    # linha começando com 'rob', inclui um robô,
    # o segundo argumento na linha é a imagem
    if r[0] == 'rob':
        #coloca a imagem do robo no robs
        robs.append(Robo(r[1]))
        #continue prossegue para a proxima iteração
        continue
    if r[0] == 'd_cel':
        i = int(r[1])
        j = int(r[2])
        t = int(r[3])
        if j == 0:
            arena.append([])
        if t == 6:
          basecount+=1
          arena[i].append(cell(i,j,t,'flag'+str(basecount)+'.png'))
        else:
          arena[i].append(cell(i,j,t,''))
        arena[i][j].draw()
        continue

    #se não for nem rob nem fim
    # linha de movimentação:
    # robô, coordenadas originais, coordenadas finais
    ri, oi, oj, di, dj = list(map(int, r))
    # redesenha a célula original (apaga o robô)
    if 0 <= oi  < W and 0 <= oj < H:
        arena[oi][oj].draw()
    # desenha o robô na célula de destino
    if 0 <= di  < W and 0 <= dj < H and ri < len(robs):
        robs[ri].draw(di,dj)

    # tempo de espera para atualização, apenas para testes
atu()
running = True
while running:
  for event in pg.event.get():
    if event.type == pg.QUIT:
      running = False


#pg.quit()
# Local variables:
# mode: python
# End:

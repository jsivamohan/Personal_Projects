import pygame
import random 
import json
from os import path

WIDTH = 1350
HEIGHT = 1000
FPS = 60
SIZE = 90
SCORE = 0
CLOCK = 0
#stop = False

# define colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)
SILVER = (192, 192, 192)

# initialize pygame and create window
pygame.init()
pygame.mixer.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Shmup!")
clock = pygame.time.Clock()
font_name = pygame.font.match_font('arial')

def draw_text(surf, text, size, x, y):
   font = pygame.font.Font(font_name,size)
   text_surface = font.render(text, True, WHITE)
   text_rect = text_surface.get_rect()
   text_rect.midtop = (x,y)
   surf.blit(text_surface, text_rect)
def new_maze(x, y, type):
   m = Maze(x, y, type)
   all_sprites.add(m)
   maze.add(m)

def rand_choice(gridx):
   r = len(gridx)
   ra = random.randrange(0,r)
   grid = gridx[ra]
   gridx.remove(grid)
   return grid

def new_level():
   grid = rand_choice(grid_list)
   respawn()
   for m in mobs:
      m.kill()
   for p in powerups:
      p.kill()
   for wall in maze:
      wall.kill()
   #gen_orbs(grid)
   for i in range(player.level*2 + 5):
      new_mob(grid,"MOB","NONE")
   new_mob(grid,"P","STOP")
   new_mob(grid,"P","COOL")
   new_mob(grid,"P", "KEY")
   new_mob(grid, "P", "BULB")
   gen_maze(grid)
   
def gen_maze(grid):
   s = len(grid)**.5
   for i in range(int(s)):
      for l in range(int(s)):
         if grid[int(i*s + l)][0] == 1:
            new_maze(i* SIZE,l* SIZE,"V")
         if grid[int(i*s + l)][1] == 1:
            new_maze(i*SIZE,l * SIZE + SIZE,"H")
         if grid[int(i*s + l)][2] == 1:
            new_maze(i*SIZE + SIZE, l * SIZE,"V")
         if grid[int(i*s + l)][3] == 1:   
            new_maze(i*SIZE,l * SIZE,"H")

def new_mob(grid,type1, type2):
   x = int(len(grid) ** .5)
   s = int(len(grid) ** .5)   
   '''
   tl = False
   tr = False
   bl = False
   br = False
   x1 = 0
   y1 = 0
   if tl == False:
      x = int(x/2)
      y = int(y/2)
      tl = True
   elif tr == False:
      x1 = int(x/2)
      y = int(y/2)
      tr = True
   elif bl == False:
      x = int(x/2)
      y1 = int(y/2)
      bl = True
   elif br == False:
      x1 = int(x/2)
      y1 = int(y/2)
      br = True
   '''
   l = random.randrange(0,s)
   w = random.randrange(0,s)
   if l==0 and w == 0:
      w += 1
   if (l==s or l==(s-1)) and (w == s or w== (s-1)):
      w -= 1
      l -= 1
   if type1 == "MOB":
      m = Mob(w*SIZE + SIZE / 2 , l*SIZE + SIZE / 2)
      all_sprites.add(m)      
      mobs.add(m)
   else:#add to main code
      #p = p_up(w*SIZE + SIZE / 2 , l*SIZE + SIZE / 2,type2)
      t = trans_obj(w*SIZE + SIZE / 2 , l*SIZE + SIZE / 2, type2)
      tros.add(t)
      all_sprites.add(t)
      #powerups.add(p)
      #all_sprites.add(p)  

def new_orb(x,y):
   o = Orb(x,y)
   all_sprites.add(o)
   Orbs.add(o)
   
def new_trans(x,y,type):#add to main code
   tro = trans_obj(x,y,type)
   tros.add(tro)
   all_sprites.add(tro)

def gen_orbs(grid):#create all the points orbs for the player to try to collect (1 per 40*40 mini box in center)
   s = int((len(grid)**0.5))
   for i in range(s):
      for l in range(s):
         if i > 0 or l > 0:
            new_orb(i*(SIZE) + SIZE / 2, l*(SIZE) + SIZE / 2)       
      
def draw_lives(surf, x, y, lives, img):
   for i in range(lives):
      img_rect = img.get_rect()
      img_rect.x = x + (30 * i)
      img_rect.y = y
      surf.blit(img, img_rect)
def respawn():
   player.rect.centerx = 870
   player.rect.bottom = HEIGHT - 125

def freeze_mobs():
   for m in mobs:
      m.stop = True
      m.check_time = pygame.time.get_ticks()
      m.first = True
   pow_freeze.play()
def unfreeze_mobs():
   pass

def show_intro_screen():
   screen.blit(background, background_rect)
   draw_text(screen, "JEEVAN'S COOL GAME", 64, WIDTH/2, HEIGHT/4)
   draw_text(screen, "Arrow keys move, WASD to fire, try to survive!", 22, WIDTH/2, HEIGHT/2)
   draw_text(screen, "Press a key to begin", 18, WIDTH/2, HEIGHT *(3/4))
   pygame.display.flip()
   waiting = True
   while waiting:
      clock.tick(FPS)
      for event in pygame.event.get():
         if event.type == pygame.QUIT:
            pygame.quit()
         if event.type == pygame.KEYUP:
            waiting = False

def show_after_screen():
   screen.blit(background, background_rect)
   if won:
      draw_text(screen, "CONGRATULATIONS", 64, WIDTH/2, HEIGHT/4)
      draw_text(screen, "YOU'VE BEATEN THE GAME!", 64, WIDTH/2, HEIGHT/4 + 100)
      draw_text(screen, "SCORE: " + str(SCORE), 22, WIDTH/2, HEIGHT/2)
   if lost:
      draw_text(screen, "YOU DIED A HORRIBLE AND PAINFUL DEATH", 40, WIDTH/2, HEIGHT/4)
      draw_text(screen, "SUCKS TO SUCK", 40, WIDTH/2, HEIGHT/4 + 100)
      draw_text(screen, "SCORE: NOTHING BECAUSE YOU'RE DEAD!", 32, WIDTH/2, HEIGHT/2)    
   draw_text(screen, "TIME: " + str(int(pygame.time.get_ticks() / 1000)), 18, WIDTH/2, HEIGHT/2 + 100)
   draw_text(screen, "Press enter to try your luck again!", 22, WIDTH/2, HEIGHT *(3/4))
   pygame.display.flip()
   waiting = True
   while waiting:
      clock.tick(FPS)
      for event in pygame.event.get():
         if event.type == pygame.QUIT:
            pygame.quit()
         if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RETURN:
               waiting = False   

def stop_info():
   draw_lives(screen, 1000, 100, 1, image_freeze)
   draw_text(screen, "STOP powerup", 18, 1100, 105)
   draw_text(screen, "press t to use, stops all mobs for 3 seconds", 18, 1150, 150)
   draw_text(screen, "if a mob is killed while stopped it will not respawn", 18, 1150, 175)

def rapid_info():
   draw_lives(screen, 1000, 300, 1, image_rapid)
   draw_text(screen, "RAPID powerup", 18, 1100, 305)
   draw_text(screen, "press r to use, removes gun cooldown for 5 seconds", 18, 1138, 350)

def bulb_info():
   draw_lives(screen, 1000, 500, 1, image_bulb)
   draw_text(screen, "LIGHT powerup", 18, 1100, 505)
   draw_text(screen, "press b to use, reveals the maze for 3 seconds", 18, 1150, 550)

def key_info():
   draw_lives(screen, 1000, 700, 1, image_key)
   draw_text(screen, "KEY", 18, 1100, 705)
   draw_text(screen, "the key unlocks the portal to let you leave the level", 18, 1145, 750)


        
#used for animation through json files
class Spritesheet:
   def __init__(self, filename):
      self.filename = filename
      self.sprite_sheet = pygame.image.load(filename).convert() 
      self.meta_data = self.filename.replace('png', 'json')
      with open(self.meta_data) as f:
         self.data = json.load(f)
      f.close()
      
   def get_sprite(self, x, y, w, h):
      sprite = pygame.Surface((w,h))
      sprite.set_colorkey(BLACK)
      sprite.blit(self.sprite_sheet, (0,0), (x,y,w,h))
      return sprite
       
   def parse_sprite(self, name):#parses through all the subdirectories of the json file and pulls out the necesary location and size values
      sprite= self.data['frames'][name]['frame']
      x,y,w,h = sprite["x"], sprite["y"], sprite["w"], sprite["h"]
      image = self.get_sprite(x,y,w,h)
      return image

class trans_obj(pygame.sprite.Sprite):#add to main code (used for powerups only)
   def __init__(self,x,y,type):
      pygame.sprite.Sprite.__init__(self)
      self.image = pygame.Surface((20,20))
      self.image.fill(RED)
      self.image.set_colorkey(RED)
      self.rect = self.image.get_rect()
      self.radius = 30
      self.rect.centerx = x
      self.rect.centery = y
      self.type = type
   def recall(self):
      p = p_up(self.rect.centerx, self.rect.centery,self.type)
      powerups.add(p)
      all_sprites.add(p)

class Player(pygame.sprite.Sprite):
   def __init__(self):
      pygame.sprite.Sprite.__init__(self)
      self.image = pygame.image.load('hero_1.png').convert()
      #self.image = pygame.Surface((30,30))
      self.image = pygame.transform.scale(self.image, (30,30))          
      self.rect = self.image.get_rect()
      self.rect.centerx = 870
      self.rect.bottom = HEIGHT - 125
      #self.rect.top  = y - 
      self.radius = 15
      self.keylast = ""
      #self.width = 50
      self.shoot_delay = 1000
      self.last_shot = pygame.time.get_ticks()
      self.lives = 3
      self.check_time = pygame.time.get_ticks()
      self.level = 0
      self.cool = False
      self.left = True
      self.right = True
      self.up = True
      self.down = True      
      self.has_stop = False
      self.has_rapid = False
      self.first = False
      self.hidden = False
      self.hide_timer = pygame.time.get_ticks()
      self.index = 0
      self.radius = 30
      self.has_key = False
      self.no_coll = False
      self.coll_check_time = pygame.time.get_ticks()
      self.off_wall = True
      self.wall_left = False
      self.wall_right = False
      self.wall_up = False
      self.wall_down = False
      self.has_bulb = False
      self.wall_comp = Maze(0,0,"H")
   def update(self):
   #unhide if hidden
      if self.hidden and pygame.time.get_ticks() - self.hide_timer > 3000:
         self.hidden = False
         respawn()
      if self.no_coll and pygame.time.get_ticks() - self.coll_check_time > 5000:
         self.no_coll = False
      '''
      if self.keylast == "RIGHT":
         self.image = player_run[self.index % len(player_run)]
      elif self.keylast == "LEFT":
         self.image = player_run[self.index % len(player_run)]
         self.image = pygame.transform.flip(self.image, True, False)
      elif self.keylast == "UP":
         self.image = player_run[self.index % len(player_run)]
         self.image = pygame.transform.rotate(self.image, 90)
      elif self.keylast == "DOWN":
         self.image = player_run[self.index % len(player_run)]
         self.image = pygame.transform.rotate(self.image, 270)         
      if pygame.time.get_ticks() % 5 == 0:
         self.index += 1
      self.image = pygame.transform.scale(self.image, (60,60))   
      '''
      keystate = pygame.key.get_pressed()
      if self.wall_left and keystate[pygame.K_RIGHT] or (self.rect.centery > (self.wall_comp).rect.bottom or self.rect.centery < (self.wall_comp).rect.top):
         self.wall_left = False
         self.left = True
      if self.wall_right and keystate[pygame.K_LEFT] or (self.rect.centery > (self.wall_comp).rect.bottom or self.rect.centery < (self.wall_comp).rect.top):
         self.wall_right = False
         self.right = True
      if self.wall_up and keystate[pygame.K_DOWN] or (self.rect.centerx > (self.wall_comp).rect.right or self.rect.centerx < (self.wall_comp).rect.left):
         self.wall_up = False
         self.up = True    
      if self.wall_down and keystate[pygame.K_UP] or (self.rect.centerx > (self.wall_comp).rect.right or self.rect.centerx < (self.wall_comp).rect.left):
         self.wall_down = False
         self.down = True      
         
      if keystate[pygame.K_LEFT] and self.left:
         self.rect.x -= 2
         self.keylast = "LEFT"
      if keystate[pygame.K_RIGHT] and self.right:
         self.rect.x += 2
         self.keylast = "RIGHT"
      if keystate[pygame.K_UP] and self.up:
         self.rect.y -= 2
         self.keylast = "UP"
      if keystate[pygame.K_DOWN] and self.down:
         self.rect.y += 2   
         self.keylast = "DOWN"
      if keystate[pygame.K_w]:
         self.shoot("UP")
      if keystate[pygame.K_a]:
         self.shoot("LEFT")
      if keystate[pygame.K_s]:
         self.shoot("DOWN")
      if keystate[pygame.K_d]:
         self.shoot("RIGHT")               
      if keystate[pygame.K_t]and self.has_stop:
         freeze_mobs()
         self.has_stop = False
      if keystate[pygame.K_r] and self.has_rapid:
         player.cool = True
         player.check_time = pygame.time.get_ticks()   
         pow_rapid_up.play()
         self.has_rapid = False    
         self.first = True  
      if keystate[pygame.K_b] and self.has_bulb:
         for m in maze:
            m.bulb_time = pygame.time.get_ticks()
            m.bulb_active = True
         self.has_bulb = False
      if self.rect.right > WIDTH:
         self.rect.right = WIDTH
      if self.rect.left < 0:
         self.rect.left = 0
      if self.rect.top < 0:
         self.rect.top = 0
      if self.rect.bottom > HEIGHT and self.hidden == False:
         self.rect.bottom = HEIGHT
      #self.left = True
      #self.right = True
      #self.up = True
      #self.down = True         
   def shoot(self,type):
      now = pygame.time.get_ticks()
      if pygame.time.get_ticks() - self.check_time > 3000:
         self.cool = False
         if self.first == True:
            pow_rapid_down.play()
            self.first = False
      if self.cool == False:
         self.shoot_delay = 1000
      else:
         self.shoot_delay = 10
      if now - self.last_shot > self.shoot_delay:
         self.last_shot = now
         bullet = Bullet(self.rect.centerx, self.rect.bottom, type)
         all_sprites.add(bullet)
         bullets.add(bullet)     
         shoot_sound.play()    
   
   def hide(self):
      self.hidden = True
      self.hide_timer = pygame.time.get_ticks()
      self.rect.center = (WIDTH / 2, HEIGHT + 200)
         
class Bullet(pygame.sprite.Sprite):
   def __init__(self, x, y, type):
      pygame.sprite.Sprite.__init__(self)
      self.image = pygame.image.load('laserRed16.png').convert()
      self.image = pygame.transform.scale(self.image, (5,30))   
      self.rect = self.image.get_rect()
      self.rect.bottom = y
      self.rect.centerx = x
      if type == "UP":
         self.speedy = -10
         self.speedx = 0
      elif type == "DOWN":
         self.speedy = 10
         self.speedx = 0
      elif type == "LEFT":
         self.image = pygame.transform.rotate(self.image, 90)
         self.speedx = -10
         self.speedy = 0
      elif type == "RIGHT":  
         self.image = pygame.transform.rotate(self.image, 270)
         self.speedx = 10
         self.speedy = 0

   def update(self):
      self.rect.y += self.speedy
      self.rect.x += self.speedx      
      # kill if it moves off the top of the screen
      if self.rect.bottom < 0:
         self.kill()
            
class Maze(pygame.sprite.Sprite):
   def __init__(self, x, y, type):
      pygame.sprite.Sprite.__init__(self)
      self.type = type
      #self.image = pygame.image.load('wall_texture.png').convert()
      self.type = type
      if self.type == "V":
         self.image = pygame.Surface((5,SIZE))#add to main code
      if self.type == "H":
         self.image = pygame.Surface((SIZE,5))#add to main code
      self.image.fill(SILVER)
      self.image.set_colorkey(SILVER)
      self.rect = self.image.get_rect()
      self.rect.left = x
      self.rect.top = y
      self.truex = x
      self.truey = y
      self.bulb_time = pygame.time.get_ticks()
      self.bulb_active = False
      self.hidden = True
   def update(self):#add to main code
      if self.bulb_active and pygame.time.get_ticks() - self.bulb_time < 3000:
         self.image.set_colorkey(BLACK)
      else:
         if self.hidden:
            self.image.set_colorkey(SILVER) 
         else:
            self.image.set_colorkey(BLACK)      
            
class End(pygame.sprite.Sprite):
   def __init__(self):
      pygame.sprite.Sprite.__init__(self)
      #self.image = pygame.image.load('end_door.png').convert()
      self.image = pygame.Surface((85,85))     
      self.image.set_colorkey(BLACK)
      self.rect = self.image.get_rect()
      self.rect.left = 5
      self.rect.bottom = 90
      self.done = False
      self.index = 0
      self.contact = False
      self.check_time = pygame.time.get_ticks()
   def update(self):
      if player.has_key:
         self.image = e_door_cycle[self.index % len(e_door_cycle)]
      elif self.contact == False:
         self.image = e_door_activate[16]
         self.index = 0
      else:
         self.image = e_door_activate[self.index % len(e_door_activate)]
      self.image = pygame.transform.scale(self.image, (85,85))
      if pygame.time.get_ticks() % 7 == 0:
         self.index += 1
         '''
      if self.contact and pygame.time.get_ticks() - self.check_time > 3000:
         self.contact = False
         new_level()
         '''
class Orb(pygame.sprite.Sprite):
   def __init__(self,x,y):
      pygame.sprite.Sprite.__init__(self)
      self.image = pygame.image.load('orb_blue.png').convert()
      self.image = pygame.transform.scale(self.image, (15,15))     
      self.image.set_colorkey(BLACK)
      #self.image.fill("GREEN")
      self.rect = self.image.get_rect()
      self.rect.centerx = x
      self.rect.centery = y
      
class p_up(pygame.sprite.Sprite):#boolean is_active controls powerups True = active
   def __init__(self,x,y,type):
      pygame.sprite.Sprite.__init__(self)
      if type == "STOP":
         self.image = pygame.image.load('mob_freeze.png').convert()
      elif type == "COOL":
         self.image = pygame.image.load('bolt_gold.png').convert()
      elif type == "KEY":
         self.image = pygame.image.load('key_update.png').convert()
      else:
         self.image = pygame.image.load('lightbulb_update.png').convert()
      self.image = pygame.transform.scale(self.image, (30,30))   
      self.image.set_colorkey(BLACK)
      self.rect = self.image.get_rect()
      self.rect.centerx = x
      self.rect.centery = y
      #self.ptime = 3000
      #self.check_time = pygame.time.get_ticks()
      #self.slow = False
      #self.stop = False
      #self.first = True
      self.type = type
      self.radius = 20
      '''
   def active(self):
      self.check_time = pygame.time.get_ticks()
      #self.is_active = False
      
   def update(self):
      if self.stop:
         if pygame.time.get_ticks() - self.check_time > 3000:
            #self.is_active = False
            #pygame.quit()
            self.stop = False
   '''
class Mob(pygame.sprite.Sprite):
   def __init__(self,x,y):
      pygame.sprite.Sprite.__init__(self)
      self.image = pygame.Surface((40,40))
      #self.image.fill("YELLOW")
      self.rect = self.image.get_rect()
      self.rect.centerx = x
      self.rect.centery = y
      self.speed = 3
      self.speedy = -1*(self.speed)
      self.speedx = 0
      self.dir = "UP"
      self.last_dir = ""
      self.framecount = 0
      self.is_change_dir = False
      self.stop = False
      self.check_time = pygame.time.get_ticks()
      #self.slow = False
      #self.s = 1
      self.first = False
      self.index = 0
      self.j_dir = "UP"
      self.no_coll = True
      self.coll_delay = pygame.time.get_ticks()
      self.dead = False
      #self.l = 0
      #self.w = 0
      #.s = 0
      '''
   def reset_pos(self,grid):
      s = int(len(grid) ** .5)  
      self.l = random.randrange(0,s)
      self.w = random.randrange(0,s)
      if self.l==0 and self.w == 0:
         self.w += 1
      if (self.l==self.s or self.l==(self.s-1)) and (self.w == self.s or self.w== (self.s-1)):
         self.w -= 1
         self.l -= 1
      self.centerx = self.w
      self.centery = self.l

   def move(self):
      self.rect.centerx = WIDTH/ 5
      self.rect.centery = HEIGHT + 10000
   '''
   def reset_dir(self):  
      self.is_change_dir = True
      self.last_dir = self.dir
      if self.last_dir == "UP":
         self.j_dir = "DOWN"
         self.speedy = self.speed    
         self.speedx = 0 
      elif self.last_dir == "DOWN":
         self.j_dir = "UP"
         self.speedy = -1*(self.speed)    
         self.speedx = 0         
      elif self.last_dir == "LEFT":
         self.j_dir = "RIGHT"
         self.speedx = self.speed    
         self.speedy = 0          
      else:
         self.j_dir = "LEFT"
         self.speedx = -1*(self.speed)    
         self.speedy = 0          
   def change_dir(self):
      self.is_change_dir = False
      '''
      r = random.randrange(0,3)#random version of the movement system for mobs
      if r == 0:
         self.dir = "UP"
         self.speedy = -5
         self.speedx = 0
      elif r == 1:
         self.dir = "DOWN"
         self.speedy = 5   
         self.speedx = 0
      elif r == 2:
         self.dir = "LEFT"
         self.speedx = -5  
         self.speedy = 0
      else:
         self.dir = "RIGHT"
         self.speedx = 5    
         self.speedy = 0
      '''
      self.last_dir = self.dir
      if self.last_dir == "UP":
         self.j_dir = "DOWN"
         self.dir = "DOWN"
         self.speedy = self.speed    
         self.speedx = 0 
      elif self.last_dir == "DOWN":
         self.j_dir = "RIGHT"
         self.dir = "RIGHT"
         self.speedy = 0    
         self.speedx = self.speed         
      elif self.last_dir == "LEFT":
         self.j_dir = "UP"
         self.dir = "UP"
         self.speedx = 0    
         self.speedy = -1*(self.speed)          
      else:
         self.j_dir = "LEFT"
         self.dir = "LEFT"
         self.speedx = -1*(self.speed)    
         self.speedy = 0   
   def reset_kill(self):
      new_mob(grid,"MOB","L")
   
   def update(self):
      if self.no_coll and pygame.time.get_ticks() - self.coll_delay > 3000:
         self.no_coll = False
         #self.reset_pos(grid)
      if self.j_dir == "DOWN":
         self.image = mob_down[(self.index % len(mob_up))]
      elif self.j_dir == "UP":
         self.image = mob_up[(self.index % len(mob_up))]
      elif self.j_dir == "LEFT":
         self.image = mob_left[(self.index % len(mob_up))]         
      else:
         self.image = mob_right[(self.index % len(mob_up))]  
      self.image = pygame.transform.scale(self.image, (40,40))    
      if pygame.time.get_ticks() % 5 == 0:
         self.index += 1 
      if pygame.time.get_ticks() - self.check_time > 3000:
         self.stop = False
         if self.first == True:
            pow_unfreeze.play()
            self.first = False     
      if self.stop == False:         
         if self.is_change_dir:
            self.framecount += 1
         '''   
         if self.slow:
            #self.s = 3
            if self.dir == "UP":
               self.speedy = -1
            elif self.dir == "DOWN":
               self.speedy = 1
            elif self.dir == "RIGHT":
               self.speedx = 1
            elif self.dir == "LEFT":
               self.speedx = -1
         #self.speedx = int(self.speedx / self.s)
         #self.speedy = int(self.speedy / self.s)   
         self.image = pygame.transform.scale(self.image, (30,30))
         '''
         if self.framecount >= (SIZE / 2)/(abs(self.speedx) + abs(self.speedy)):
            self.framecount = 0
            self.change_dir()
         self.rect.centerx += self.speedx
         self.rect.centery += self.speedy

#grid = [[1,0,0,1],[1,0,0,0],[1,0,1,0],[1,1,0,0],[0,1,1,1],[0,0,1,1],[1,1,0,0],[0,1,1,1],[1,0,0,1]
        #,[1,1,1,0],[0,0,0,1],[1,1,1,0],[0,0,1,1],[0,0,1,0],[0,0,1,0],[1,1,1,0]]
'''
grid = [[1,0,0,1],[1,1,0,0],[1,0,0,1],[1,0,1,0],[1,0,0,0],[1,1,0,0],[0,1,0,1],[0,1,1,1],
         [0,1,0,1],[1,1,0,1],[0,1,0,1],[0,1,0,1],[1,1,0,1],[1,0,0,1],[0,0,0,0],[0,1,1,0],[0,1,0,1],
         [0,1,1,1],[0,1,0,1],[0,1,0,1],[0,1,0,1],[1,1,0,1],[0,0,1,1],[1,0,1,0],[0,1,0,1],[0,1,0,1],
         [0,0,1,1],[0,0,1,0],[1,0,0,1],[1,1,0,0],[0,0,1,1],[0,0,1,0],[1,0,1,0],[1,0,1,0],[0,1,1,0],[0,1,1,1]]

grid = [[1,0,0,1],[1,0,1,0],[1,1,0,0],[0,0,1,1],[1,0,0,0],[0,1,1,1,0],[1,0,0,1],[1,1,0,0],
        [0,0,1,1],[1,1,0,0],[0,0,0,1],[1,0,1,0],[0,0,1,0],[1,1,0,0],[0,1,0,1],[0,1,0,1],
        [1,0,0,1],[0,1,1,0],[0,0,1,1],[1,0,0,0],[1,1,0,0],[0,1,0,1],[0,1,0,1],[0,1,0,1],
        [0,0,0,1],[1,0,1,1],[1,0,1,0],[0,1,0,0],[0,1,1,1],[0,0,1,1],[0,1,1,0],[0,1,1,1],
        [0,0,1,1],[1,0,0,0],[1,1,0,0],[0,1,1,1],[1,0,0,1],[1,0,1,0],[1,0,1,0],[1,0,1,0],
        [1,0,0,1],[0,1,1,0],[0,0,1,1],[1,0,1,0],[0,1,1,0],[1,1,0,1],[1,0,0,1],[1,1,0,0],
        [0,0,1,1],[1,0,1,0],[1,0,1,0],[1,0,1,0],[1,0,0,0],[0,0,1,0],[0,1,1,0],[0,1,0,1],
        [1,0,1,1],[1,0,1,0],[1,0,1,0],[1,0,1,0],[0,1,1,0],[1,0,1,1],[1,0,1,0],[0,1,1,0]]
        '''

#loading images
image_freeze = pygame.image.load('mob_freeze.png').convert()
image_rapid =  pygame.image.load('bolt_gold.png').convert()  
image_bulb = pygame.image.load('lightbulb_update.png').convert()
image_key = pygame.image.load('key_update.png').convert()
image_key = pygame.transform.scale(image_key, (30,30))
#Load the background image
background = pygame.image.load('starfield.png').convert()
background = pygame.transform.scale(background, (920,1000))     
background_rect = background.get_rect()

#sounds
shoot_sound = pygame.mixer.Sound('laser1.wav')   
shoot_sound.set_volume(0.5)
orb_sound = pygame.mixer.Sound('ding_sound.mp3')      
orb_sound.set_volume(0.05)
pow_freeze = pygame.mixer.Sound('pow_freeze.wav')
pow_freeze.set_volume(0.1)
pow_unfreeze = pygame.mixer.Sound('pow_unfreeze.mp3')
pow_unfreeze.set_volume(0.1)
pow_rapid_up = pygame.mixer.Sound('rapid_up.wav')
pow_rapid_up.set_volume(0.1)
pow_rapid_down = pygame.mixer.Sound('rapid_down.ogg')
pow_rapid_down.set_volume(0.1)
mob_death = pygame.mixer.Sound('mob_death.wav')
mob_death.set_volume(0.1)
player_death = pygame.mixer.Sound('player_death.mp3')
player_death.set_volume(0.1)

#spirtesheet and json for animation

my_spritesheet = Spritesheet('mob_spritesheet.png')
mob_up = [my_spritesheet.parse_sprite('walk_up_1'), my_spritesheet.parse_sprite('walk_up_2'),
          my_spritesheet.parse_sprite('walk_up_3')]
mob_down = [my_spritesheet.parse_sprite('walk_down_1'), my_spritesheet.parse_sprite('walk_down_2'),
            my_spritesheet.parse_sprite('walk_down_3')]
mob_left = [my_spritesheet.parse_sprite('walk_left_1'), my_spritesheet.parse_sprite('walk_left_2'),
            my_spritesheet.parse_sprite('walk_left_3')]
mob_right = [my_spritesheet.parse_sprite('walk_right_1'), my_spritesheet.parse_sprite('walk_right_2'),
            my_spritesheet.parse_sprite('walk_right_3')]
'''
player_run_ss = Spritesheet('player_run.png')
player_run = [player_run_ss.parse_sprite('run_1'), player_run_ss.parse_sprite('run_2'), player_run_ss.parse_sprite('run_3'), 
              player_run_ss.parse_sprite('run_4'), player_run_ss.parse_sprite('run_5'), player_run_ss.parse_sprite('run_6'), 
              player_run_ss.parse_sprite('run_7'),player_run_ss.parse_sprite('run_8'), player_run_ss.parse_sprite('run_9'), player_run_ss.parse_sprite('run_10')]
'''
e_door_cycle_ss = Spritesheet('end_door_cycle.png')
e_door_cycle = [e_door_cycle_ss.parse_sprite('sprite1'), e_door_cycle_ss.parse_sprite('sprite2'), e_door_cycle_ss.parse_sprite('sprite3'),
                e_door_cycle_ss.parse_sprite('sprite4'), e_door_cycle_ss.parse_sprite('sprite5')] 
                
e_door_activate_ss = Spritesheet('end_door_activate.png')
e_door_activate = [e_door_activate_ss.parse_sprite('sprite1'), e_door_activate_ss.parse_sprite('sprite2'), e_door_activate_ss.parse_sprite('sprite3'),
                  e_door_activate_ss.parse_sprite('sprite4'), e_door_activate_ss.parse_sprite('sprite5'), e_door_activate_ss.parse_sprite('sprite6'), 
                  e_door_activate_ss.parse_sprite('sprite7'), e_door_activate_ss.parse_sprite('sprite8'), e_door_activate_ss.parse_sprite('sprite9'), 
                  e_door_activate_ss.parse_sprite('sprite10'), e_door_activate_ss.parse_sprite('sprite11'), e_door_activate_ss.parse_sprite('sprite12'), 
                  e_door_activate_ss.parse_sprite('sprite13'), e_door_activate_ss.parse_sprite('sprite14'), e_door_activate_ss.parse_sprite('sprite15'), 
                  e_door_activate_ss.parse_sprite('sprite16'), e_door_activate_ss.parse_sprite('sprite17')]#sprite 17 is the black door for when all orbs have not been collected 

game_over = True   
running = True
won = False
lost = False
first = True
#stop = False
while running:
   if game_over:
      if first:
         show_intro_screen()
      else:
         show_after_screen()
      first = False
      game_over = False
      all_sprites = pygame.sprite.Group()
      maze = pygame.sprite.Group()
      bullets = pygame.sprite.Group()
      mobs = pygame.sprite.Group()
      powerups = pygame.sprite.Group()
      Orbs = pygame.sprite.Group()
      tros = pygame.sprite.Group()
      player = Player()
      all_sprites.add(player)
      end = End()
      all_sprites.add(end)     
      grid1 = [[1,0,1,1], [1,1,0,0], [1,0,0,1], [1,0,1,0], [1,1,0,0], [1,0,0,1], [1,0,1,0], [1,0,0,0], [1,0,1,0], [1,1,1,0], 
               [1,0,0,1], [0,1,0,0], [0,0,1,1], [1,1,0,0], [0,1,0,1], [0,1,0,1], [1,1,0,1], [0,1,0,1], [1,0,0,1], [1,1,0,0], 
               [0,1,0,1], [0,0,0,1], [1,0,0,0], [0,1,1,0], [0,0,1,1], [0,1,1,0], [0,0,1,1], [0,1,1,0], [0,1,0,1], [0,1,0,1], 
               [0,1,1,1], [0,1,0,1], [0,0,1,1], [1,0,1,0], [1,0,1,0], [1,0,1,0], [1,0,1,0], [1,0,1,0], [0,0,0,0], [0,1,1,1],
               [1,0,0,1], [0,0,1,0], [1,0,1,0], [1,0,1,0], [1,0,1,0], [1,1,0,0], [1,0,1,1], [1,1,0,0], [0,0,1,1], [1,1,0,0], 
               [0,0,1,1], [1,1,1,0], [1,0,0,1], [1,0,1,0], [1,1,0,0], [0,1,0,1], [1,0,0,1], [0,0,1,0], [1,1,0,0], [0,1,1,1], 
               [1,0,0,1], [1,0,0,0], [0,1,1,0], [1,1,0,1], [0,1,0,1], [0,0,1,1], [0,1,1,0], [1,0,0,1], [0,0,1,0], [1,1,1,0], 
               [0,1,0,1], [0,0,0,1], [1,0,1,0], [0,1,1,0], [0,1,0,1], [1,0,1,1], [1,0,1,0], [0,0,1,0], [1,0,1,0], [1,1,0,0],
               [0,1,0,1], [0,0,1,1], [1,1,0,0], [1,1,0,1], [0,0,1,1], [1,0,0,0], [1,0,0,0], [1,1,0,0], [1,0,0,1], [0,1,0,0],
               [0,0,1,1], [1,1,1,0], [0,0,1,1], [0,1,1,0], [1,0,1,1], [0,1,1,0], [0,1,1,1], [0,0,1,1], [0,1,1,0], [0,1,1,1]]
               
      grid2 = [[1,1,0,1], [1,0,0,1], [1,1,0,0], [1,0,1,1], [1,0,1,0], [1,0,1,0], [1,1,0,0], [1,1,0,1], [1,0,0,1], [1,1,1,0],
               [0,1,0,1], [0,1,0,1], [0,0,1,1], [1,0,1,0], [1,1,0,0], [1,0,0,1], [0,1,1,0], [0,1,0,1], [0,0,1,1], [1,1,0,0],
               [0,0,0,1], [0,1,1,0], [1,0,0,1], [1,1,0,0], [0,0,1,1], [0,0,1,0], [1,1,1,0], [0,1,0,1], [1,0,0,1], [0,1,0,0],
               [0,0,1,1], [1,0,1,0], [0,1,1,0], [0,0,1,1], [1,0,1,0], [1,1,0,0], [1,0,0,1], [0,1,0,0], [0,1,0,1], [0,1,0,1],
               [1,0,0,1], [1,1,1,0], [1,0,0,1], [1,0,0,0], [1,0,0,0], [0,1,0,0], [0,1,0,1], [0,1,1,1], [0,1,0,1], [0,1,0,1],
               [0,1,0,1], [1,0,0,1], [0,1,1,0], [0,1,0,1], [0,1,1,1], [0,0,1,1], [0,1,1,0], [1,0,0,1], [0,1,1,0], [0,1,1,1],
               [0,0,1,1], [0,1,1,0], [1,0,0,1], [0,0,1,0], [1,0,1,0], [1,0,0,0], [1,1,0,0], [0,1,0,1], [1,0,1,1], [1,1,0,0], 
               [1,0,0,1], [1,1,0,0], [1,0,0,1], [1,0,1,0], [1,0,1,0], [0,1,1,0], [0,0,1,1], [0,0,1,0], [1,0,0,0], [0,1,1,0], 
               [0,1,0,1], [0,0,1,1], [0,1,1,0], [1,0,1,1], [1,1,0,0], [1,0,0,1], [1,0,1,0], [1,0,1,0], [0,1,1,0], [1,1,0,1], 
               [0,0,1,1], [1,0,1,0], [1,0,1,0], [1,0,1,0], [0,1,1,0], [0,0,1,1], [1,0,1,0], [1,0,1,0], [1,0,1,0], [0,1,1,0]]
      
      grid3 = [[1,0,1,1], [1,0,1,0], [1,0,1,0], [1,0,0,0], [1,0,1,0], [1,0,1,0], [1,0,1,0], [1,0,1,0], [1,1,0,0], [1,1,0,1], 
               [0,0,1,1], [1,0,1,0], [1,0,1,0], [1,1,0,0], [1,0,1,1], [1,1,0,0], [1,0,1,1], [1,1,0,0], [0,0,0,1], [0,1,1,0],
               [1,0,1,1], [1,0,0,0], [1,1,1,0], [0,0,1,1], [1,1,0,0], [0,0,0,1], [1,0,0,0], [0,1,1,0], [0,0,1,1], [1,1,0,0], 
               [1,0,0,1], [0,0,1,0], [1,0,1,0], [1,1,0,0], [0,1,0,1], [0,1,1,1], [0,1,0,1], [1,0,0,1], [1,1,0,0], [0,1,0,1], 
               [0,1,1,1], [1,1,0,0], [1,0,0,1], [0,1,0,0], [0,1,0,1], [1,0,0,1], [0,1,1,0], [0,1,0,1], [0,1,0,1], [0,1,0,1], 
               [1,0,0,1], [0,0,1,0], [0,0,1,0], [0,1,1,0], [0,1,0,1], [0,0,1,1], [1,0,0,0], [0,1,0,0], [0,1,0,1], [0,1,0,1],
               [0,1,0,1], [1,0,0,1], [1,0,1,0], [1,0,1,0], [0,1,0,0], [1,0,0,1], [0,1,1,0], [0,1,0,1], [0,0,0,1], [0,1,0,0],
               [0,0,1,1], [0,0,1,0], [1,1,1,0], [1,0,0,1], [0,1,1,0], [0,1,1,1], [1,0,0,1], [0,1,1,0], [0,1,0,1], [0,1,0,1],
               [1,0,0,1], [1,0,1,0], [1,1,0,0], [0,0,1,1], [1,0,0,0], [1,1,0,0], [0,1,0,1], [1,1,0,1], [0,1,0,1], [0,1,0,1],
               [0,0,1,1], [1,1,1,0], [0,0,1,1], [1,0,1,0], [0,1,1,0], [0,0,1,1], [0,1,1,0], [0,0,1,1], [0,1,1,0], [0,1,1,1]]
      
      grid4 = [[1,1,0,1], [1,0,0,1], [1,1,0,0], [1,0,0,1], [1,0,1,0], [1,0,1,0], [1,0,1,0], [1,0,1,0], [1,0,1,0], [1,1,0,0], 
               [0,0,1,1], [0,1,1,0], [0,1,0,1], [0,0,1,1], [1,0,1,0], [1,1,1,0], [1,0,0,1], [1,0,1,0], [1,1,0,0], [0,1,0,1], 
               [1,0,0,1], [1,1,1,0], [0,1,0,1], [1,0,0,1], [1,0,0,0], [1,0,1,0], [0,1,1,0], [1,1,0,1], [0,0,1,1], [0,1,0,0],
               [0,0,1,1], [1,0,1,0], [0,0,0,0], [0,1,1,0], [0,0,0,1], [1,0,1,0], [1,1,0,0], [0,1,0,1], [1,1,0,1], [0,1,0,1],
               [1,0,0,1], [1,1,0,0], [0,0,0,1], [1,1,0,0], [0,0,1,1], [1,1,0,0], [0,1,0,1], [0,1,0,1], [0,0,1,1], [0,1,1,0],
               [0,1,0,1], [0,0,1,1], [0,1,1,0], [0,1,0,1], [1,0,0,1], [0,1,1,0], [0,0,1,1], [0,1,1,0], [1,1,0,1], [1,1,0,1],
               [0,0,0,1], [1,0,1,0], [1,1,0,0], [0,1,0,1], [0,0,1,1], [1,0,1,0], [1,0,1,0], [1,0,1,0], [0,1,1,0], [0,1,0,1],
               [0,0,0,1], [1,1,1,0], [0,1,0,1], [0,0,1,1], [1,0,1,0], [1,0,0,0], [1,1,1,0], [1,0,0,1], [1,0,1,0], [0,1,1,0],
               [0,1,0,1], [1,0,0,1], [0,0,1,0], [1,0,1,0], [1,1,0,0], [0,1,1,1], [1,0,0,1], [0,1,0,0], [1,0,0,1], [1,1,0,0],
               [0,1,1,1], [0,0,1,1], [1,0,1,0], [1,1,1,0], [0,0,1,1], [1,0,1,0], [0,1,1,0], [0,0,1,1], [0,1,1,0], [0,1,1,1]] 
               
      grid5 =  [[1,0,1,1], [1,0,1,0], [1,0,1,0], [1,1,0,0], [1,0,0,1], [1,0,0,0], [1,0,1,0], [1,0,1,0], [1,0,1,0], [1,1,0,0],
               [1,0,0,1], [1,0,1,0], [1,1,0,0], [0,0,1,1], [0,1,1,0], [0,0,0,1], [1,1,0,0], [1,0,0,1], [1,1,0,0], [0,1,1,1],
               [0,1,1,1], [1,0,0,1], [0,1,1,0], [1,0,0,1], [1,0,1,0], [0,1,1,0], [0,0,0,1], [0,1,1,0], [0,0,1,1], [1,1,0,0],
               [1,0,0,1], [0,1,1,0], [1,0,0,1], [0,1,1,0], [1,0,0,1], [1,1,0,0], [0,0,1,1], [1,0,1,0], [1,1,0,0], [0,1,0,1],
               [0,0,1,1], [1,0,1,0], [0,1,0,0], [1,0,1,1], [0,1,1,0], [0,0,1,1], [1,0,1,0], [1,0,0,0], [0,1,1,0], [0,1,1,1],
               [1,0,1,1], [1,0,1,0], [0,1,0,0], [1,0,0,1], [1,0,1,0], [1,0,1,0], [1,1,0,0], [0,0,1,1], [1,1,1,0], [1,1,0,1],
               [1,0,0,1], [1,0,1,0], [0,1,1,0], [0,1,0,1], [1,0,0,1], [1,1,0,0], [0,0,1,1], [1,0,1,0], [1,0,1,0], [0,1,1,0],
               [0,1,0,1], [1,0,0,1], [1,0,1,0], [0,1,1,0], [0,1,0,1], [0,0,1,1], [1,0,1,0], [1,1,0,0], [1,0,0,1], [1,1,0,0],
               [0,1,0,1], [0,1,0,1], [1,0,0,1], [1,1,0,0], [0,1,1,1], [1,0,0,1], [1,1,0,0], [0,0,1,1], [0,1,1,0], [0,1,0,1],
               [0,0,1,1], [0,0,1,0], [0,1,1,0], [0,0,1,1], [1,0,1,0], [0,1,1,0], [0,0,1,1], [1,0,1,0], [1,0,1,0], [0,1,1,0]]
      
      grid_list = [grid1, grid2, grid3, grid4, grid5]
      grid = rand_choice(grid_list)
      gen_maze(grid)
      #gen_orbs(grid)
      for i in range(5):
         new_mob(grid,"MOB","NONE")
      new_mob(grid,"P","STOP")
      new_mob(grid,"P","COOL")      
      new_mob(grid,"P","KEY")
      new_mob(grid,"P","BULB")
      SCORE = 0
   
   clock.tick(FPS)
   for event in pygame.event.get():
      # check for closing window
      if event.type == pygame.QUIT:
         running = False   
   walls = pygame.sprite.spritecollide(player, maze, False, False)
   keystate = pygame.key.get_pressed()
   for wall in walls:
         player.wall_arr = wall
         if keystate:
            if player.rect.centerx <= wall.rect.left and (wall.rect.top < player.rect.centery < wall.rect.bottom):#wall on RIGHT
               player.wall_right = True
               player.right = False
               #player.down = True
               player.rect.x -= 2
               '''
               if player.rect.y < wall.rect.bottom:
                  player.rect.y = wall.rect.bottom 
               '''      
            #else:
               #player.up = True  
            elif player.rect.centerx >= wall.rect.right and (wall.rect.top < player.rect.centery < wall.rect.bottom):#wall on LEFT 
               player.wall_left = True
               player.left = False
               #player.up = True
               player.rect.x += 2 
               '''    
               if player.rect.y + 30 > wall.rect.top:
                  player.rect.y = wall.rect.top - 30 
               '''   
            #else:
               #player.down = True
            elif player.rect.centery <= wall.rect.top and (wall.rect.left < player.rect.centerx < wall.rect.right):#wall on BOTTOM   
               player.wall_down = True
               player.down = False
               #player.left = True
               player.rect.y -= 2
               '''
               if player.rect.x + 30 > wall.rect.left:
                  player.rect.x = wall.rect.left - 30
               '''   
            #else:
               #player.right = True
            elif player.rect.centery >= wall.rect.bottom and (wall.rect.left < player.rect.centerx < wall.rect.right):#wall on TOP
               player.wall_up = True
               player.up = False
               #player.right = True
               player.rect.y += 2
               '''       
               if player.rect.x < wall.rect.right:
                  player.rect.x = wall.rect.right
               '''   
            #else:
               #player.left = True
   if player.has_key:
      if pygame.sprite.collide_mask(player, end):
         player.has_key = False
         #end.contact = True
         player.level += 1
         new_level()
         #end.check_time = pygame.time.get_ticks()
   if player.level >= 3:
      game_over = True
      won = True
      lost = False
   b = pygame.sprite.groupcollide(maze, bullets , False, True)
   Mob_coll = pygame.sprite.groupcollide(mobs, maze, False, False)
   for m in Mob_coll:
      m.reset_dir()
   Bullet_coll = pygame.sprite.groupcollide(mobs, bullets, True, True)
   for b in Bullet_coll:
      #b.move()
      mob_death.play()
      if b.stop == False:
         b.dead = True
         b.res_delay = pygame.time.get_ticks()
         b.reset_kill()
      SCORE += 50 
   Play_coll = pygame.sprite.spritecollide(player, mobs, False)
   for p in Play_coll:
      if player.no_coll == False and p.no_coll == False:   
         player.no_coll = True
         player.coll_check_time = pygame.time.get_ticks()  
         player_death.play()
         player.hide()
         player.lives -= 1
         #new_mob(grid,"MOB","L")
   if player.lives == 0:
      game_over = True
      won = False
      lost = True
   P_up_coll = pygame.sprite.spritecollide(player, powerups, False)
   for p in P_up_coll:
      #p.check_time = pygame.time.get_ticks()
      #p.stop = True
      if p.type == "STOP" and player.has_stop == False:
         player.has_stop = True
      elif p.type == "COOL" and player.has_rapid == False:
         player.has_rapid = True
      elif p.type == "KEY" and player.has_key == False:
         player.has_key = True
      elif p.type == "BULB" and player.has_bulb == False:
         player.has_bulb = True
      #powerup_info(p.type, pygame.time.get_ticks())
      p.kill()   
   Orb_coll = pygame.sprite.spritecollide(player, Orbs, True)
   for o in Orb_coll:
      SCORE += 5  
      orb_sound.play()
   fog_coll_w = pygame.sprite.spritecollide(player, maze, False, pygame.sprite.collide_circle)#collision between circle values for fog of war implementation (add to main code)
   for f in fog_coll_w:
      f.hidden = False   
   if len(Orbs) == 0:
      end.done = True  
   fog_coll_p = pygame.sprite.spritecollide(player, tros, True, pygame.sprite.collide_circle)#add to main code
   for f in fog_coll_p:
      f.recall()      
   
   '''
   for p in powerups:
      if p.stop:
         for m in mobs:
            m.stop = True  
   for p in powerups:
      if p.slow:
         for m in mobs:
            m.slow = True
   '''
   #stop = False   
   # Update
   all_sprites.update()  
  
  #draw / render
   screen.fill(BLACK)
   screen.blit(background, background_rect)
   all_sprites.draw(screen)
   draw_lives(screen, 100, HEIGHT - 50, player.lives, player.image)
   if player.has_stop:
      draw_lives(screen, 820, HEIGHT - 50, 1, image_freeze)
      #stop_info()
      #powerup_info("STOP")
   if player.has_rapid:
      draw_lives(screen, 870, HEIGHT - 50, 1, image_rapid)
      #rapid_info()
      #powerup_info("COOL")
   if player.has_bulb:
      draw_lives(screen, 770, HEIGHT - 50, 1, image_bulb)
      #bulb_info()
      #powerup_info("BULB")
   draw_text(screen, str(SCORE), 18, 460, HEIGHT - 50)   
   draw_text(screen, "LEVEL: " + str(player.level+1), 18, 300, HEIGHT - 50)
   draw_text(screen, "CLOCK: " + str(int(pygame.time.get_ticks() / 1000)), 18, 600, HEIGHT - 50)
pygame.quit()

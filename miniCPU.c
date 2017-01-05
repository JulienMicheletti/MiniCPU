/*
 * -------------------------- TP du module Archi1 -------------------------
 *
 * ATTENTION : un outil de détection de plagiat logiciel sera utilisé lors de la correction,
 * vous avez donc tout intérêt à effectuer un travail PERSONNEL
 *
 * Un mot/registre de 10 à 30 bits est représenté par un tableau d'entiers égaux à 0 ou 1.
 * Une ALSU est représentée par une structure ALSU, avec registre accumulateur
 * et registre d'état.
 * Un CPU (très très simplifié) est représenté par une ALSU et quelques registres
 * nécessaires pour stocker les résultats intermédiaires.
 *
 * Certaines fonctions vous sont fournies, d'autres sont à implanter ou à compléter,
 * de préférence dans l'ordre où eles sont indiquées.
 * Il vous est fortement conseillé de lire attentivement l'ensemble des commentaires.
 *
 * Parmi les opérations arithmétiques et logiques, seules 4 opérations de base sont
 * directement fournies par l'ALSU, les autres doivent être décrites comme des algorithmes
 * travaillant à l'aide des opérateurs de base de l'ALSU simplifiée et pouvant utiliser les 
 * registres du CPU.
 *
 * La fonction main() vous permet de tester au fur et à mesure les fonctions que vous implantez.
 *
 * ----------------------------------------------------------------------------------------------
 *
 * author: B. Girau
 * version: 2016-2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/////////////////////////////////////////////////////////
// définition de types
/////////////////////////////////////////////////////////

typedef struct {
  int size;
  int *word;
} Register;

typedef struct {
  Register accu;
  int* flags; // indicateurs Z C O N
} ALSU;

typedef struct {
  ALSU alsu;
  Register R0;
  Register R1;
  Register R2;
} CPU;

/////////////////////////////////////////////////////////
// fonctions d'initialisation
/////////////////////////////////////////////////////////

/*
 * allocation d'un mot/registre entier de 10 à 30 bits initialisé à 0
 */
Register reg(int size) {
  Register R;
  int	i = 0;

  R.size = size;
  R.word = malloc(sizeof(int) * size);
  while (i < size)
    {
      R.word[i] = 0;
      i++;
    }
  return R;
}

/*
 * Initialisation du mot (mot de 10 à 30 bits, codant un entier en Cà2) avec une valeur entière.
 */
void setValue(Register R,int n) { 
  int dec = 0;
  int i = 0;
  int j = 0;

  while (i < R.size)
    {
      R.word[i] = 0;
      i++;
    }
  i = 0;
  if (n < 0)
    {
      dec = 1;
      n = n * -1;
    }
  while (n != 0)
    {
      R.word[i] = n % 2;
      n = n / 2;
      i++;
    }
  if (dec == 1)
    {
      i = 0;
      while (i < R.size && R.word[i] != 1)
  	i++;
      for (j = i; j < R.size; j++)
  	{
  	  if (R.word[j] == 1)
  	    R.word[i] = 1;
  	  else
  	    R.word[i] = 0;
  	}
    }
}


/*
 * instanciation d'un mot de 10 à 30 bits initialisé avec la valeur n
 */
Register initR(int size,int n) {
  Register R=reg(size);
  setValue(R,n) ;
  return R;
}
    
/*
 * Initialisation du mot (mot de 10 à 30 bits) par recopie des bits du mot en paramètre.
 */
void copyValue(Register R,Register src) {
  int	i = 0;

  setValue(R, 0);
  while (i < src.size)
    {
      R.word[i] = src.word[i];
      i++;
    }
}

/*
 * instanciation d'un mot de 10 à 30 bits initialisé par recopie d'un mot
 */
Register copyRegister(Register R) {
  Register C;

  C = reg(R.size);
  copyValue(C, R);
  return C;
}

/*
 * initialise l'ALSU
 */
ALSU initALSU(int size) {
  ALSU res;
  res.accu=reg(size);
  res.flags=(int*)malloc(4*sizeof(int));
  return res;
}

/*
 * initialise le CPU
 */
CPU initCPU(int size) {
  CPU res;
  res.alsu=initALSU(size);
  res.R0=reg(size);
  res.R1=reg(size);
  res.R2=reg(size);
  return res;
}

/////////////////////////////////////////////////////////
// fonctions de lecture
/////////////////////////////////////////////////////////

/*
 * Retourne la valeur entière signée représentée par le mot (complément à 2).
 */    
int intValue(Register R) {
  int	val = 0;
  int	j;

  for (j = 0; j < R.size - 1; j++)
    val = val + R.word[j] * (1 << j);
  val = val - R.word[j] * (1 << j);
  return val;
}

/*
 * Retourne une chaîne de caractères décrivant les 10 à 30 bits
 */
char* toString(Register R) {
  int i,j=0;
  char* s=(char*)malloc(40*sizeof(char));
  for (i=R.size-1;i>=0;i--) {
    if (R.word[i]==1) s[j]='1';
    else s[j]='0';
    j++;
  }
  return s;
}

/*
 * Retourne l'écriture des indicateurs associés à l'ALSU.
 */
char* flagsToString(ALSU alsu) {
  char *string=(char*)malloc(10*sizeof(char));
  sprintf(string,"z%dc%do%dn%d",alsu.flags[0],alsu.flags[1],alsu.flags[2],alsu.flags[3]);
  return string;
}

/*
 * affiche à l'écran le contenu d'une ALSU
 */
void printing(ALSU alsu) {
  printf("Le mot est = %s\n", toString(alsu.accu));
  printf("La taille de l'alsu est égale à %d\n", alsu.accu.size);
  printf("Les indicateurs associés à l'ALSU sont %s\n", flagsToString(alsu));
  
}

/////////////////////////////////////////////////////////
// fonctions de manipulations élémentaires
/////////////////////////////////////////////////////////

/*
 * Positionne l'indicateur Z en fonction de l'état de l'accumulateur
 */
void setZ(ALSU alsu) {
  int	i = 0;

  while (i < alsu.accu.size)
    {
      if (alsu.accu.word[i] = 1)
	{
	  alsu.flags[0] = 0;
	  return;
	}
      i++;
    }
  alsu.flags[0] = 1;
}

/////////////////////////////////////////////////////////
// opérateurs de base de l'ALSU
/////////////////////////////////////////////////////////

/*
 * Stocke le paramètre dans le registre accumulateur
 */
void pass(ALSU alsu,Register B) {
  copyValue(alsu.accu, B);
  setZ(alsu);
  alsu.flags[1] = 0;
  alsu.flags[2] = 0;
  if (intValue(B) < 0)
    alsu.flags[3] = 1;
  else
    alsu.flags[3] = 0;
}

/*
 * Effectue un NAND (NON-ET) entre le contenu de l'accumulateur et le paramètre.
 */
void nand(ALSU alsu,Register B) {
  int	i = 0;

  while (i < alsu.accu.size)
    {
      if (alsu.accu.word[i] == 1 && B.word[i] == 1)
	alsu.accu.word[i] = 0;
      else
	alsu.accu.word[i] = 1;
      i++;
    }
  setZ(alsu);
  alsu.flags[1] = 0;
  alsu.flags[2] = 0;
  if (intValue(alsu.accu) < 0)
    alsu.flags[3] = 1;
  else
    alsu.flags[3] = 0;
}

/*
 * Décale le contenu de l'accumulateur de 1 bit vers la droite
 */
void shift(ALSU alsu) {
  int	i;

  for (i = 0; i < alsu.accu.size; i++)
    alsu.accu.word[i] = alsu.accu.word[i+1];
  alsu.accu.word[alsu.accu.size] = 0;
  setZ(alsu);
  alsu.flags[1] = 0;
  alsu.flags[2] = 0;
  if (intValue(alsu.accu) < 0)
    alsu.flags[3] = 1;
  else
    alsu.flags[3] = 1;
}

/*
 * module Full Adder : a+b+cin = s + 2 cout
 * retourne un tableau contenant s et cout
 */
int* fullAdder(int a,int b,int cin) {
  int* res=(int*)malloc(2*sizeof(int));

  if (a == 0 && b == 0 && cin == 0)
    {
      res[0] = 0;
      res[1] = 0;
    }
  else if (a == 0 && b == 0 && cin == 1)
    {
      res[0] = 1;
      res[1] = 0;
    }
  else if (a == 0 && b == 1 && cin == 0)
    {
      res[0] = 1;
      res[1] = 0;
    }
  else if (a == 1 && b == 0 && cin == 0)
    {
      res[0] = 1;
      res[1] = 0;
    }
   else if (a == 0 && b == 1 && cin == 1)
     {
       res[0] = 0;
       res[1] = 1;
     }
   else if (a == 1 && b == 0 && cin == 1)
     {
       res[0] = 0;
       res[1] = 1;
     }
   else if (a == 1 && b == 1 && cin == 0)
    {
      res[0] = 0;
      res[1] = 1;
    }
   else if (a == 1 && b == 1 && cin == 1)
     {
       res[0] = 1;
       res[1] = 1;
     }
   return res;
}

/*
 * Additionne le paramètre au contenu de l'accumulateur (addition entière Cà2).
 * Les indicateurs sont positionnés conformément au résultat de l'opération.
 */
void add(ALSU alsu,Register B) {
  int i = 0, j = 0;
  int val, cmp = 0, *op;
  
  val = intValue(alsu.accu);
  while (i < alsu.accu.size)
    {
      if (B.word[i] != '\0')
  	j = B.word[i];
      else
  	j = 0;
      op = fullAdder(alsu.accu.word[i], j, cmp);
      alsu.accu.word[i] = op[0];
      cmp = op[1];
      free(op);
      i++;
    }
  setZ(alsu);
}

////////////////////////////////////////////////////////////////////
// Opérations logiques :
////////////////////////////////////////////////////////////////////

/*
 * Négation.
 */
void not(CPU cpu){
  nand(cpu.alsu, cpu.alsu.accu);
}

/*
 * Et.
 */
void and(CPU cpu,Register B) {
  nand(cpu.alsu, B);
  nand(cpu.alsu, cpu.alsu.accu);
}


/*
 * Ou.
 */
void or(CPU cpu,Register B) {
  copyValue(cpu.R0, cpu.alsu.accu);
  nand(cpu.alsu, B);
  copyValue(cpu.R1, cpu.alsu.accu);
  copyValue(cpu.alsu.accu, cpu.R0);
  nand(cpu.alsu, cpu.R0);
  copyValue(cpu.R2, cpu.alsu.accu);
  copyValue(cpu.alsu.accu, cpu.R1);
  nand(cpu.alsu, B);
  nand(cpu.alsu, cpu.R2);
}

/*
 * Xor.
 */
void xor(CPU cpu,Register B) {
  copyValue(cpu.R0, cpu.alsu.accu);
  not(cpu);
  or(cpu, B);
  copyValue(cpu.R1, cpu.alsu.accu);
  copyValue(cpu.alsu.accu, B);
  not(cpu);
  or(cpu, cpu.R0);
  nand(cpu.alsu, cpu.R1);
}

/*
 * Décale le receveur de |n| positions.
 * Le décalage s'effectue vers la gauche si n>0 vers la droite dans le cas contraire.
 * C'est un décalage logique (pas de report du bit de signe dans les positions 
 * libérées en cas de décalage à droite).
 * Les indicateurs sont positionnés avec le dernier bit "perdu".
 */
void logicalShift(CPU cpu,int n) {
  if (n > 0){
    while (n > 0){
      add(cpu.alsu, cpu.alsu.accu);
      n--;
    }
  }else if (n < 0){
    n = n * -1;
    while (n > 0){
      shift(cpu.alsu);
      n--;
    }
  }
}

/////////////////////////////////////////////////////////
// Opérations arithmétiques entières
/////////////////////////////////////////////////////////

/*
 * Opposé.
 */
void opp(CPU cpu) {
  not(cpu);
  setValue(cpu.R0, 1);
  add(cpu.alsu, cpu.R0);
}

/*
 * Soustraction.
 */
void sub(CPU cpu,Register B) {
  copyValue(cpu.R1, cpu.alsu.accu);
  copyValue(cpu.alsu.accu, B);
  opp(cpu);
  add(cpu.alsu, cpu.R1);
}

/*
 * Multiplication.
 */
void mul(CPU cpu,Register B) {
  int i, j;

  setValue(cpu.R0, 0);
  copyValue(cpu.R1, cpu.alsu.accu);
  for (i = 0; i <= B.size; i++){
    if (B.word[i] == 1){
      for (j = 0; j != i; j++){
	add(cpu.alsu, cpu.alsu.accu);
      }
      add(cpu.alsu, cpu.R0);
      copyValue(cpu.R0, cpu.alsu.accu);
      copyValue(cpu.alsu.accu, cpu.R1);
    }
  }
  copyValue(cpu.alsu.accu, cpu.R0);
}

/////////////////////////////////////////////////////////
// Programme de test
/////////////////////////////////////////////////////////

int main(int argc,char *argv[]) {
  
  // Champs privés
  
  Register operand;
  ALSU alsu;
  CPU cpu;
  
  int chosenInt,integer ;
  int go_on = 1 ;
  
  char* menu =
    "              Programme de test\n\n0  Quitter\n1  setValue(operande,int)\n2  pass(alsu,operande)\n3  printing(alsu)\n4  afficher toString(operande)\n5  afficher intValue(operande)\n6  afficher intValue(accu)\n7  accu=add(accu,operande)\n8 accu=sub(accu,operande)\n9  accu=and(accu,operande)\n10 accu=or(accu,operande)\n11 accu=xor(accu,operande)\n12 accu=not(accu)\n13 accu=opp(accu)\n14 accu=logicalShift(accu,int)\n15 accu=mul(accu,operande)\n\n" ;

  char* invite = "--> Quel est votre choix  ? " ;
  
  printf("%s",menu) ; 

  operand=reg(20);
  cpu=initCPU(20);
  alsu=cpu.alsu;
  
  while (go_on==1) {
    printf("%s",invite);
    scanf("%d",&chosenInt);
    switch (chosenInt) {
    case 0 : 
      go_on = 0 ;
      break ;
    case 1 :
      printf("Entrez un nombre :"); 
      scanf("%d",&integer);
      setValue(operand,integer);
      break ;
    case 2 : 
      pass(alsu,operand);
      break ;
    case 3 : 
      printing(alsu);
      break ;
    case 4 : 
      printf("%s\n",toString(operand));
      break ;
    case 5 : 
      printf("intValue(operand)=%d\n",intValue(operand));
      break ;
    case 6 : 
      printf("intValue(accu)=%d\n",intValue(alsu.accu));
      break ;
    case 7 : 
      add(alsu,operand);
      printf("apres add(), accu = ");
      printing(alsu);
      break ;
    case 8 : 
      sub(cpu,operand);
      printf("apres sub(), A = ");
      printing(alsu);
      break ;
    case 9 : 
      and(cpu,operand);
      printf("apres and(), A = ");
      printing(alsu);
      break ;
    case 10 : 
      or(cpu,operand);
      printf("apres or(), A = ");
      printing(alsu);
      break ;
    case 11 : 
      xor(cpu,operand);
      printf("apres xor(), A = ");
      printing(alsu);
      break ;
    case 12 : 
      not(cpu);
      printf("apres not(), A = ");
      printing(alsu);
      break ;
    case 13 : 
      opp(cpu);
      printf("apres opp(), A = ");
      printing(alsu);
      break ;
    case 14 : 
      printf("Entrez un entier :") ;
      scanf("%d",&integer);
      logicalShift(cpu,integer);
      printf("apres logicalshift(), A = ");
      printing(alsu);
      break ;
    case 15 : 
      mul(cpu,operand);
      printf("apres mul(), A = ");
      printing(alsu);
      break ;
    default : 
      printf("Choix inexistant !!!!\n");
      printf("%s",menu);
    }
  }
  printf("Au revoir et a bientot\n");
  return 0;
}

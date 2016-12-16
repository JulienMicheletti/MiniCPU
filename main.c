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
  int dec = 1;
  int i = 0;

  R.word[0] = n % 2;
  for (i = 1; i < R.size; i++)
    {
      if (n > 1)
	{
	  n /= 2;
	  dec = n % 2;
	  R.word[i] = dec;
	}
      else
	R.word[i] = 0;
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

  for (i = 0; i < src.size; i++)
    R.word[i] = src.word[i];
}

/*
 * instanciation d'un mot de 10 à 30 bits initialisé par recopie d'un mot
 */
Register copyRegister(Register R) {
  Register C;
  int	i;

  C = reg(R.size);
  copyValue(C, R);
  /* for (i = 0; i < R.size; i++) */
  /*   C.word[i] = R.word[i]; */
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
  int	val1 = 0;
  int	j;

  for (j = 0; j < R.size - 1; j++)
    val1 += R.word[j] * (1 << j);
  val1 -= R.word[j] * (1 << j);
  return val1;
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
  printf("SIZE ALSU = %d\n", alsu.accu.size);
  printf("WORD ALSU = %s\n", toString(alsu.accu));
  printf("%s\n", flagsToString(alsu));
  
}

/////////////////////////////////////////////////////////
// fonctions de manipulations élémentaires
/////////////////////////////////////////////////////////

/*
 * Positionne l'indicateur Z en fonction de l'état de l'accumulateur
 */
void setZ(ALSU alsu) {
  int	i;

  for (i = 0; i < alsu.accu.size; i++)
    if (alsu.accu.word[i] = 1)
      {
	alsu.flags[0] = 0;
	return;
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
  alsu.accu.word[i] = 0;
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
  // à compléter
}

////////////////////////////////////////////////////////////////////
// Opérations logiques :
////////////////////////////////////////////////////////////////////

/*
 * Négation.
 */
void not(CPU cpu){
  // à compléter
}

/*
 * Et.
 */
void and(CPU cpu,Register B) {
  // à compléter
}


/*
 * Ou.
 */
void or(CPU cpu,Register B) {
  // à compléter
}

/*
 * Xor.
 */
void xor(CPU cpu,Register B) {
  // à compléter
}

/*
 * Décale le receveur de |n| positions.
 * Le décalage s'effectue vers la gauche si n>0 vers la droite dans le cas contraire.
 * C'est un décalage logique (pas de report du bit de signe dans les positions 
 * libérées en cas de décalage à droite).
 * Les indicateurs sont positionnés avec le dernier bit "perdu".
 */
void logicalShift(CPU cpu,int n) {
  // à compléter
}

/////////////////////////////////////////////////////////
// Opérations arithmétiques entières
/////////////////////////////////////////////////////////

/*
 * Opposé.
 */
void opp(CPU cpu) {
  // à compléter
}

/*
 * Soustraction.
 */
void sub(CPU cpu,Register B) {
  // à compléter
}

/*
 * Multiplication.
 */
void mul(CPU cpu,Register B) {
  // à compléter
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
    "              Programme de test\n\n0  Quitter\n1  setValue(operande,int)\n2  pass(alsu,operande)\n3  printing(alsu)\n4  afficher toString(operande)\n5  afficher intValue(operande)\n6  afficher intValue(accu)\n8  accu=add(accu,operande)\n9  accu=sub(accu,operande)\n10  accu=and(accu,operande)\n11 accu=or(accu,operande)\n12 accu=xor(accu,operande)\n13 accu=not(accu)\n14 accu=opp(accu)\n15 accu=logicalShift(accu,int)\n17 accu=mul(accu,operande)\n\n" ;
  
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

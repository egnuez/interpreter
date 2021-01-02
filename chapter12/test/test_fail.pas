PROGRAM Part12;
VAR
  a, b : INTEGER;
  y    : REAL;

  PROCEDURE P1;
  VAR
  
      a : REAL;
      k : INTEGER;
      PROCEDURE P2;
      VAR
        a, z : INTEGER;
      BEGIN {P2}
        z := 777;
      END;  {P2}
  
  BEGIN {P1}
      k := 1;
  END;  {P1} 

BEGIN {Part12}
  a := 2;
  b := 10 * r + 10 * a DIV 4;
  y := 20 / 7 + 3.14;
END.  {Part12}
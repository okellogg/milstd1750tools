/***************************************************************************/
/*                                                                         */
/* Project   :        sim1750 -- Mil-Std-1750 Software Simulator           */
/*                                                                         */
/* Component :     arith.c -- simulate 1750A arithmetic operations         */
/*                                                                         */
/* Copyright :         (C) Daimler-Benz Aerospace AG, 1994-97              */
/*                         (C) 2017 Oliver M. Kellogg                      */
/* Contact   :           okellogg@users.sourceforge.net                    */
/*                                                                         */
/* Disclaimer:                                                             */
/*                                                                         */
/*  This program is free software; you can redistribute it and/or modify   */
/*  it under the terms of the GNU General Public License as published by   */
/*  the Free Software Foundation; either version 2 of the License, or      */
/*  (at your option) any later version.                                    */
/*                                                                         */
/*  This program is distributed in the hope that it will be useful,        */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of         */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          */
/*  GNU General Public License for more details.                           */
/*                                                                         */
/*  You should have received a copy of the GNU General Public License      */
/*  along with this program; if not, write to the Free Software            */
/*  Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.   */
/*                                                                         */
/***************************************************************************/

#include "arch.h"
#include "status.h"
#include "utils.h"
#include "flt1750.h"

#define FLT_1750_EPSILON     1.1920928955078125000E-7
#define FLT_1750_MAX         1.70141163178059628080016879768632819712E38
#define FLT_1750_MIN         \
 1.469367938527859384960920671527807097273331945965109401885939632848E-39

#define DBL_1750_EPSILON     1.818989403545856475830078125000E-12
#define DBL_1750_MAX         1.70141183460159746721865958647159324672E38
#define DBL_1750_MIN         \
 1.469367938527859384960920671527807097273331945965109401885939632848E-39

bool update_pir = TRUE;  /* evaluated in function arith() */

/************* utilities for Condition Status in the Status Word *************/

void
update_cs (short *operand, datatype data_type)
{
  bool is_zero;
  ushort sw_save = simreg.sw & 0x8FFF;

  switch (data_type)
    {
    case VAR_INT:
      is_zero = (operand[0] == 0);
      break;
    case VAR_LONG:
    case VAR_FLOAT:
      is_zero = (operand[0] == 0 && operand[1] == 0);
      break;
    case VAR_DOUBLE:
      is_zero = (operand[0] == 0 && operand[1] == 0 && operand[2] == 0);
      break;
    }
  if (is_zero)
    simreg.sw = sw_save | CS_ZERO;
  else if (*operand & 0x8000)  /* Check sign bit. Same for all data types. */
    simreg.sw = sw_save | CS_NEGATIVE;
  else
    simreg.sw = sw_save | CS_POSITIVE;
}


void
compare (datatype data_type, short *operand0, short *operand1)
{
  ushort sw_save = simreg.sw & 0x0FFF;
  short   op0,  op1;
  int    lop0, lop1;
  double fop0, fop1;

  switch (data_type)
    {
    case VAR_INT:
      op0 = operand0[0];
      op1 = operand1[0];
      if (op0 < op1)
        simreg.sw = sw_save | CS_NEGATIVE;
      else if (op0 > op1)
        simreg.sw = sw_save | CS_POSITIVE;
      else
        simreg.sw = sw_save | CS_ZERO;
      break;
    case VAR_LONG:
      lop0 = ((int) operand0[0] << 16) | ((int) operand0[1] & 0xFFFF);
      lop1 = ((int) operand1[0] << 16) | ((int) operand1[1] & 0xFFFF);
      if (lop0 < lop1)
        simreg.sw = sw_save | CS_NEGATIVE;
      else if (lop0 > lop1)
        simreg.sw = sw_save | CS_POSITIVE;
      else
        simreg.sw = sw_save | CS_ZERO;
      break;
    case VAR_FLOAT:
      fop0 = from_1750flt (operand0);
      fop1 = from_1750flt (operand1);
      if (fop0 < fop1)
        simreg.sw = sw_save | CS_NEGATIVE;
      else if (fop0 > fop1)
        simreg.sw = sw_save | CS_POSITIVE;
      else
        simreg.sw = sw_save | CS_ZERO;
      break;
    case VAR_DOUBLE:
      fop0 = from_1750eflt (operand0);
      fop1 = from_1750eflt (operand1);
      if (fop0 < fop1)
        simreg.sw = sw_save | CS_NEGATIVE;
      else if (fop0 > fop1)
        simreg.sw = sw_save | CS_POSITIVE;
      else
        simreg.sw = sw_save | CS_ZERO;
      break;
    }
}


/*****************************************************************************/

/* See enum operation_kind in file arch.h :   */
static char *operation_name[] = { "ADD", "SUB", "MULS", "MUL", "DIVV", "DIV" };

/* add,subtract,multiply,divide -- for all 1750A data types.
   operand0 and operand1 are the input operands (vectors of shorts).
   The `vartyp' argument determines how the input vectors are interpreted.
   The result is stored in operand0.
   The condition code bits (CPZN) in simreg.sw are updated accordingly.
   The Pending Interrupt Register (simreg.pir) is also updated in case of
   under/overflow conditions during computation.
 */

void
arith (operation_kind operation,
       datatype vartyp,    /* Always specify data type of SECOND operand! */
       short *operand0, short *operand1)
{
  simreg.sw &= ~CS_CARRY;

  switch (vartyp)
    {
    case VAR_INT:
      switch (operation)
        {
        case ARI_ADD:
        case ARI_SUB:
          {
            ushort uop0 = (ushort) *operand0;
            ushort uop1 = (ushort) *operand1;
            ushort uaccu;
            bool is_add = (operation == ARI_ADD), sign_comparison;

            if (is_add)
              {
                uaccu = uop0 + uop1;
                sign_comparison = ((uop0 & 0x8000) == (uop1 & 0x8000));
              }
            else
              {
                uaccu = uop0 - uop1;
                sign_comparison = ((uop0 & 0x8000) != (uop1 & 0x8000));
              }

            if (sign_comparison && (uop0 & 0x8000) != (uaccu & 0x8000))
              {
                simreg.sw |= CS_CARRY;
                if (update_pir)
                  {
                    simreg.pir |= INTR_FIXOFL;
                    int lop0 = (int) *operand0, lop1 = (int) *operand1;
                    info ("FIXOFL on integer %s, op0=%hd op1=%hd res=%d",
                          is_add ? "ADD" : "SUB", *operand0, *operand1,
                          is_add ? lop0 + lop1 : lop0 - lop1);
                  }
              }

            operand0[0] = uaccu;
            update_cs (operand0, VAR_INT);
          }

        elsecase ARI_MUL:
          {
            /* Single precision integer multiply with 32-bit result */
            int lop0 = (int) *operand0;
            int lop1 = (int) *operand1;
            int laccu;

            laccu = lop0 * lop1;

            operand0[0] = (ushort) ((laccu >> 16) & 0xFFFF);
            operand0[1] = (ushort) (laccu & 0xFFFF);
            update_cs (operand0, VAR_LONG);
          }

        elsecase ARI_MULS:
          {
            int lop0 = (int) *operand0;
            int lop1 = (int) *operand1;
            int laccu;
            bool overflow = FALSE;

            laccu = lop0 * lop1;

            if (laccu < -32768)
              {
                overflow = TRUE;
              }
            else if (laccu > 32767)
              {
                overflow = TRUE;
              }

            if (overflow)
              {
                simreg.pir |= INTR_FIXOFL;
                info ("FIXOFL on integer MULS, op0=%d op1=%d res=%d",
                      lop0, lop1, laccu);
              }

            operand0[0] = (short) laccu;
            update_cs (operand0, VAR_INT);
          }

        elsecase ARI_DIV:
          {
            /* 16 bit divide with 32-bit dividend */
            int lop0 = ((int) operand0[0] << 16)
                     | ((int) operand0[1] & 0xFFFF);
            int lop1 = (int) *operand1;

            if (lop1 == 0)
              {
                simreg.pir |= INTR_FIXOFL;
                info ("FIXOFL on integer DIV, op0=%d op1=%d", lop0, lop1);
              }
            else
              {
                int laccu = lop0 / lop1;
                bool overflow = FALSE;

                if (laccu < -32768)
                  {
                    overflow = TRUE;
                  }
                else if (laccu > 32767)
                  {
                    overflow = TRUE;
                  }

                if (overflow)
                  {
                    simreg.pir |= INTR_FIXOFL;
                    info ("FIXOFL on integer DIV, op0=%d op1=%d", lop0, lop1);
                  }
                else
                  {
                    int rem = lop0 % lop1;
                    operand0[0] = (short) laccu;
                    operand0[1] = (short) rem;
                    update_cs (operand0, VAR_INT);
                  }
              }
          }

        elsecase ARI_DIVV:
          {
            /* 16 bit signed divide with 16-bit dividend */
            int lop0 = (int) *operand0;
            int lop1 = (int) *operand1;

            if (lop1 == 0)
              {
                simreg.pir |= INTR_FIXOFL;
                info ("FIXOFL on integer DIVV, op0=%d op1=%d res=%d", lop0, lop1);
              }
            else
              {
                int laccu = lop0 / lop1;
                bool overflow = FALSE;

                if (laccu < -32768)
                  {
                    overflow = TRUE;
                  }
                else if (laccu > 32767)
                  {
                    overflow = TRUE;
                  }

                if (overflow)
                  {
                    simreg.pir |= INTR_FIXOFL;
                    info ("FIXOFL on integer DIV, op0=%d op1=%d", lop0, lop1);
                  }
                else
                  {
                    int rem = lop0 % lop1;
                    operand0[0] = (short) laccu;
                    operand0[1] = (short) rem;
                    update_cs (operand0, VAR_INT);
                  }
              }
          }
        }

    elsecase VAR_LONG:
      if (operation == ARI_ADD || operation == ARI_SUB)
        {
          uint ulop0 = ((uint) operand0[0] << 16)
                     | ((uint) operand0[1] & 0xFFFF);
          uint ulop1 = ((uint) operand1[0] << 16)
                     | ((uint) operand1[1] & 0xFFFF);
          uint ulaccu;
          bool sign_comparison;

          if (operation == ARI_ADD)
            {
              ulaccu = ulop0 + ulop1;
              sign_comparison = ((ulop0 & 0x80000000) == (ulop1 & 0x80000000));
            }
          else
            {
              ulaccu = ulop0 - ulop1;
              sign_comparison = ((ulop0 & 0x80000000) != (ulop1 & 0x80000000));
            }

          if (sign_comparison && (ulop0 & 0x80000000) != (ulaccu & 0x80000000))
            {
              simreg.sw |= CS_CARRY;
              simreg.pir |= INTR_FIXOFL;
              info ("FIXOFL on long %s, op0=%d op1=%d res=%d",
                    (operation == ARI_ADD) ? "addition" : "subtraction",
                    (int) ulop0, (int) ulop1, (int) ulaccu);
            }

          operand0[0] = (short) (ulaccu >> 16);
          operand0[1] = (short) (ulaccu & 0x0000FFFF);
          update_cs (operand0, VAR_LONG);
        }
      else  /* ARI_MUL or ARI_DIV */
        {
          long long lop0 = ((long long) operand0[0] << 16)
                         | ((long long) operand0[1] & 0xFFFFLL);
          long long lop1 = ((long long) operand1[0] << 16)
                         | ((long long) operand1[1] & 0xFFFFLL);
          long long laccu;
          bool overflow = FALSE;

          if (operation == ARI_MUL)
            {
              laccu = lop0 * lop1;

              if (lop0 != 0LL && lop1 != 0LL)
                {
                  if ((lop0 & 0x80000000LL) == (lop1 & 0x80000000LL))
                    {
                      if ((laccu & 0xFFFFFFFF80000000LL) != 0LL)
                        overflow = TRUE;
                    }
                  else
                    {
                      if ((laccu & 0xFFFFFFFF80000000LL) != 0xFFFFFFFF80000000LL)
                        overflow = TRUE;
                    }
                }

              if (overflow)
                {
                  simreg.pir |= INTR_FIXOFL;
                  info ("FIXOFL on long multiply, op0=%lld op1=%lld res=%lld",
                        lop0, lop1, laccu);
                }

              operand0[0] = (short) (laccu >> 16);
              operand0[1] = (short) (laccu & 0xFFFFLL);
              update_cs (operand0, VAR_LONG);
            }
          else  /* ARI_DIV */
            {
              if (lop1 == 0LL ||
                  ((unsigned long long) lop0 == 0xFFFFFFFF80000000ULL
                   && lop1 == -1LL))
                {
                  simreg.pir |= INTR_FIXOFL;
                  info ("FIXOFL on long division, op0=%lld op1=%lld", lop0, lop1);
                }
              else
                {
                  long long rem = lop0 % lop1;
                  laccu = lop0 / lop1;
                  if (rem < 0LL && lop0 >= 0LL)
                    laccu--;
                  else if (rem > 0LL && lop0 < 0LL)
                    laccu++;
                  operand0[0] = (short) (laccu >> 16);
                  operand0[1] = (short) (laccu & 0xFFFFLL);
                  update_cs (operand0, VAR_LONG);
                }
            }
        }

    elsecase VAR_FLOAT:
      {
        bool overflow = FALSE;
        const double fop0 = from_1750flt (operand0);
        const double fop1 = from_1750flt (operand1);
        double faccu;

        switch (operation)
          {
          case ARI_ADD:
            faccu = fop0 + fop1;
          elsecase ARI_SUB:
            faccu = fop0 - fop1;
          elsecase ARI_MUL:
            faccu = fop0 * fop1;
          elsecase ARI_DIV:
            if ((fop1 < 0.0 && -fop1 < FLT_1750_MIN)
             || (fop1 >= 0.0 && fop1 < FLT_1750_MIN))
              overflow = TRUE;
            else
              faccu = fop0 / fop1;
            break;
          default:
            problem ("illegal operation code supplied to arith VAR_FLOAT");
          }

        if (overflow)
          {
            simreg.pir |= INTR_FLTOFL;
            operand0[0] = 0x0000;
            operand0[1] = 0x0000;

            info ("arith: FLT zero divide during %s,  op0=%g op1=%g res=%g",
                  operation_name[(int) operation], fop0, fop1, faccu);
          }
        else
          {
            int stat = to_1750flt (faccu, operand0);

            if (stat > 0)
              {
                simreg.pir |= INTR_FLTOFL;
                operand0[0] = 0x7FFF;
                operand0[1] = 0xFF7F;
              }
            else if (stat < 0)
              {
                simreg.pir |= INTR_FLTUFL;
                operand0[0] = 0x4000;
                operand0[1] = 0x0080;
              }

            if (stat != 0)
              info ("arith: FLT%cFL during %s,  op0=%g op1=%g res=%g",
                    (stat > 0 ? 'O' : 'U'),
                    operation_name[(int) operation], fop0, fop1, faccu);

            update_cs (operand0, VAR_FLOAT);
          }
      }

    elsecase VAR_DOUBLE:
      {
        bool overflow = FALSE;
        const double fop0 = from_1750eflt (operand0);
        const double fop1 = from_1750eflt (operand1);
        double faccu;

        switch (operation)
          {
          case ARI_ADD:
            faccu = fop0 + fop1;
          elsecase ARI_SUB:
            faccu = fop0 - fop1;
          elsecase ARI_MUL:
            faccu = fop0 * fop1;
          elsecase ARI_DIV:
            if ((fop1 < 0.0 && -fop1 < DBL_1750_MIN)
             || (fop1 >= 0.0 && fop1 < DBL_1750_MIN))
              overflow = TRUE;
            else
              faccu = fop0 / fop1;
            break;
          default:
            problem ("illegal operation code supplied to arith VAR_DOUBLE");
          }

        if (overflow)
          {
            simreg.pir |= INTR_FLTOFL;
            operand0[0] = 0x0000;
            operand0[1] = 0x0000;
            operand0[2] = 0x0000;

            info ("arith: FLT zero divide during %s,  op0=%g op1=%g res=%g",
                  operation_name[(int) operation], fop0, fop1, faccu);
          }
        else
          {
            int stat = to_1750eflt (faccu, operand0);

            if (stat > 0)
              {
                simreg.pir |= INTR_FLTOFL;
                operand0[0] = 0x7FFF;
                operand0[1] = 0xFF7F;
                operand0[2] = 0xFFFF;
              }
            else if (stat < 0)
              {
                simreg.pir |= INTR_FLTUFL;
                operand0[0] = 0x0000;
                operand0[1] = 0x0000;
                operand0[2] = 0x0000;
              }

            if (stat != 0)
              info ("arith: FLT%cFL during %s,  op0=%g op1=%g res=%g",
                    (stat > 0 ? 'O' : 'U'),
                    operation_name[(int) operation], fop0, fop1, faccu);

            update_cs (operand0, VAR_DOUBLE);
          }
      }
  }

}


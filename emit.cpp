//*******************************************************
// Purpose: Utility routines used for code generation
//
// Author: Philip Howard
// Email:  phil.howard@oit.edu
//
// Date: 2/25/2016
//
//*******************************************************

#include <iostream>
#include <stdio.h>
#include <string>
#include "emit.h"

using std::string;

void RewriteFunctions();

// Output files
static FILE* Output;
static FILE* Functions;
static FILE* Active;

static int  Next_Label = 1;

#define PRELUDE \
    "#include <stdio.h>\n" \
    "#define MEM_SIZE  10000\n" \
    "char Memory[MEM_SIZE];\n" \
    "int Frame_Pointer = 0;\n" \
    "int Stack_Pointer = 0;\n" \
    "int Temp = 0;\n" \
    "double Temp_F = 0.0;\n" \
    "int main() {\n"

#define POSTLUDE "return 0;\n }\n"

//*****************************************
// Open output files and write the prefix
bool InitOutput(const std::string filename)
{
    Output = fopen(filename.c_str(), "w");
    if (Output == NULL)
    {
        std::cerr << "Unable to open output file." << std::endl;
        return false;
    }

    Functions = tmpfile();
    if (Functions == NULL)
    {
        std::cerr << "Unable to open functions output file." << std::endl;
        return false;
    }

    Active = Output;

    fprintf(Active, "%s", PRELUDE);

    return true;
}
//*****************************************
// output postfix, copy functions to tail of main file and close files
bool FinalizeOutput()
{
    fprintf(Active, "%s", POSTLUDE);

    RewriteFunctions();

    fclose(Output);
    fclose(Functions);

    return true;
}
//*****************************************
// Generate a unique label for GOTO statements
std::string GenerateLabel()
{
    Next_Label++;
    std::string label("LABEL_$");
    label += std::to_string(Next_Label);
    return label;
}
//*****************************************
void StartFunctionOutput(string func_name)
{
    Active = Functions;
    fprintf(Active, "int %s()\n{\n", func_name.c_str());
}
//*****************************************
void EndFunctionOutput()
{
    fprintf(Active, "return 0;\n}\n");
    Active = Output;
}
//*****************************************
void RewriteFunctions()
{
    char buff[1000];

    rewind(Functions);

    while (fgets(buff, sizeof(buff), Functions) != NULL)
    {
        fputs(buff, Output);
    }
}



//*****************************************
// write an integer constant to the output
void EmitInt(int val)
{
    fprintf(Active, " %d ", val);
}
//*****************************************
// write a float constant to the output
void EmitFloat(double val)
{
    fprintf(Active, " %f ", val);
}
//*****************************************
void EmitString(std::string str)
{
    fprintf(Active, "%s", str.c_str());
}
//*****************************************
void EmitPrintTemp()
{
    fprintf(Active, "printf(\"%%d\\n\", Temp);\n");
}
//*****************************************
void EmitPrintTemp_F()
{
    fprintf(Active, "printf(\"%%6.4f\\n\", Temp_F);\n");
}

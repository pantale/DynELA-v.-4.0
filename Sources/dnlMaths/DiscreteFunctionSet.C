/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

/*
  \file DiscreteFunctionSet.C
  Definition file for the discrete function set class

  This file is the definition file for the discrete function set class.
  \ingroup dnlMaths
*/

#include <Defines.h>
#include <DiscreteFunctionSet.h>

// Constructor of the DiscreteFunctionSet class
//-----------------------------------------------------------------------------
DiscreteFunctionSet::DiscreteFunctionSet()
//-----------------------------------------------------------------------------
{
}

// Copy constructor of the DiscreteFunctionSet class
//-----------------------------------------------------------------------------
DiscreteFunctionSet::DiscreteFunctionSet(const DiscreteFunctionSet &X)
//-----------------------------------------------------------------------------
{
}

// Destructor of the DiscreteFunctionSet class
//-----------------------------------------------------------------------------
DiscreteFunctionSet::~DiscreteFunctionSet()
//-----------------------------------------------------------------------------
{
}

// Reads the set of discrete functions on the disk
/*
  - st name of the file to read from
*/
//-----------------------------------------------------------------------------
void DiscreteFunctionSet::read(String st)
//-----------------------------------------------------------------------------
{
    FILE *pfile;
    String line;
    double x, y;
    int number = 0;
    bool end = false;
    char str[100];
    long firstOne;

    firstOne = curves.size();

    if ((pfile = fopen(st.c_str(), "r")) == NULL)
    {
        fatalError("Can't open file", "for curves");
    }

    // read the first line
    line.scanFileLine(pfile);

    // scan if it's a DynELA format
    if (line.c_str()[0] == '#')
    {
        line = line.afterItem("#");
        line.strip();

        // scan the DynELA logo
        sscanf(line.c_str(), "%s", str);
        if (strcmp(str, "DynELA_plot") == 0)
        {
            line.scanFileLine(pfile);
            line = line.afterItem(":");
            line.strip();
            while (!end)
            {
                sscanf(line.c_str(), "%s", str);
                if (line.contains(" ") == true)
                {
                    line = line.afterItem(" ");
                    line.strip();
                }
                else
                {
                    end = true;
                }
                DiscreteFunction *f = new DiscreteFunction;
                f->name = str; // f->name = str;
                curves << f;
                number++;
            }
        }
    }
    else
    {
        // scan for number of terms
        sscanf(line.c_str(), "%lf", &x);
        line = line.afterItem(" ");
        line.strip();
        while (!end)
        {
            sscanf(line.c_str(), "%lf", &y);
            if (line.contains(" ") == true)
            {
                line = line.afterItem(" ");
                line.strip();
            }
            else
            {
                end = true;
            }
            DiscreteFunction *f = new DiscreteFunction;
            f->add(x, y);
            sprintf(str, "curve_%d", number + 1);
            f->name = str; // f->name = str;
            curves << f;
            number++;
        }
    }

    while (line.scanFileLine(pfile) != false)
    {
        line.popReal(x);
        for (long i = 0; i < number; i++)
        {
            line.popReal(y);
            curves(i + firstOne)->add(x, y);
        }
    }

    fclose(pfile);
}

// Writes the set of discrete functions on the disk
/*
  - filename name of the file to write
*/
//-----------------------------------------------------------------------------
void DiscreteFunctionSet::write(String filename)
//-----------------------------------------------------------------------------
{
    String nn;

    for (long j = 0; j < curves.size(); j++)
    {

        if (filename.contains(plotFileExtension))
        {
            nn = filename.beforeItem(plotFileExtension);
        }
        else
        {
            nn = filename;
        }
        nn += "_" + curves(j)->name + (plotFileExtension);

        curves(j)->toGnuplot(nn);
    }
}

// Get a discrete function from the set by it's name
/*
  Return : The corresponding discrete function from the set
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunctionSet::get(String name)
//-----------------------------------------------------------------------------
{
    for (long i = 0; i < curves.size(); i++)
    {
        if (curves(i)->name == name) // if (curves(i)->name == name)
        {
            return curves(i);
        }
    }

    return NULL;
}

// Get the ith discrete function from the set
/*
  Return : The corresponding discrete function from the set
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunctionSet::get(long i)
//-----------------------------------------------------------------------------
{
    return curves(i);
}

//-----------------------------------------------------------------------------
void DiscreteFunctionSet::add(DiscreteFunction *df)
//-----------------------------------------------------------------------------
{
    curves << df;
}

//-----------------------------------------------------------------------------
void DiscreteFunctionSet::remove(long i)
//-----------------------------------------------------------------------------
{
    curves.del(i, i);
}

//-----------------------------------------------------------------------------
void DiscreteFunctionSet::remove(String name)
//-----------------------------------------------------------------------------
{
    for (long i = 0; i < curves.size(); i++)
    {
        if (curves(i)->name == name)
        {
            curves.del(i, i);
            return;
        }
    }
}

// Number of discrete functions in the set
/*
  Return : Number of discrete functions in the set
*/
//-----------------------------------------------------------------------------
long DiscreteFunctionSet::size()
//-----------------------------------------------------------------------------
{
    return curves.size();
}

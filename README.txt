// SNAPE Version 1.0 (January 2013)
// Copyright 2006-2013 Michael D. nowak
// email: michaeldnowak@gmail.com
//	Currently at the Institute of Systematic Botany, University of Zürich
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

SNAPE stands for Single Node Age Prior Estimation. This software was produced to calculate informative divergence time priors based on a relatively complete assessment of the fossil record of a single clade of interest. The user must have an estimate of the extant standing diversity (in number of genera) for their clade of interest. To execute the software, navgate to the directory with the executable and simply type:

./SNAPEv1.0

Upon start-up, the user is asked for the name of their clade of interest (taxon), which should also be the exact name of the input file (minus file extension).

"please enter the name of your taxon or clade of interest:"

Upon user entry the program retrieves and reads into memory the pertinent fossil occurrence data from the input file with that name. This taxon name should not include a file extension. For example, if one would like to analyze the sample primate data set ("Rodentia.csv") provided with this version of the software, one would simply type "Rodentia" at the command prompt followed by return. 

A previous version of the software offered the user the option of querying the Paleobiology Database (PBDB; www.paleodb.org) for fossil occurrence data of the clade of interest, but this functionality has been disabled in the current version of the software. It is possible that at some point in the future I will try to get this working again. 

A few remarks about the input file format. Given that the original source for occurrence data was envisioned as the PBDB, our input file format is optimized for this format. You can see an example of this input file format in the example file "Rodentia.csv" that you should have received with this file. A few lines of this file representing 3 fossil occurences are shown below:
 
collection_no,occurrences.genus_reso,occurrences.genus_name,original.genus_reso,original.genus_name,collections.stage,collections.ma_max,collections.ma_min
3256,,Pantrogna,,,,55.800,37.200
9070,,Treposciurus,,,Priabonian,37.200,33.900
9071,,Tarnomys,,,Priabonian,37.200,33.900

In this example the first line is simply a header showing what each column represents. The first column shows the PBDB collection number, the third column shows the fossil genus name. 

Next the software asks the user for an estimate of the number of extant lineages, which in this case are meant to be genera. See Nowak et al. (under review) for an explanation of this constant and how it is used to calculate informative divergence time priors.  

"please enter your estimate of the number of extant genera:"

Again, here you simply type an integer and hit return. Upon user entry, the program reports several statistics about the fossil occurrence data contained in the input file. These include the number of occurrences retained, the number of lineages (i.e. fossil genera) identified, and a list of the fossil linages with the corresponding stratigraphic range. Perhaps the most interesting output is rates of origination (birth) and extinction (death) for each stratigraphic bin, which in this version of the software are shown as ISC (Gradstein) Stages. In future versions of the software, I plan to allow the user to choose the most appropriate binning scheme from a selected set including the PBDB 10 million year bins, or perhaps even a user-defined binning scheme tailored to a specific clade. But for now, the software only works with ISC stages.

The software then provides more summary statistics of the occurrence data and finally offers the user with a choice regarding the preservation rate estimate needed by the method.

"Would you like to use the FreqRat method to estimate the preservation rate (R)" "of this clade? Choose option 1 or 2."
"1. Yes, estimate the preservation rate with FreqRat"
"2. No, you would rather provide a specific preservation rate estimate."

You can choose either to provide an estimate of the average clade preservation rate, or have the software estimate a preservation rate based on your data using the FreqRat method of Foote and Raup (1997). Again, simply type 1 or 2 and hit return. 

If you choose the FreqRat method and it does not provide a meaningful preservation rate estimate (i.e. negative, or greater than 1.0), the program will arbitrarily set the preservation probability to 0.1 and then will tell you on the screen that it has set the value arbitrarily. The FreqRat makes many assumptions about the nature of your data, and encourage users of the method to understand this approach before blindly applying a preservation rate based on the FreqRat. In my experience, the FreqRat preservation rate estimates are quite unpredictable.

Program Output
Below is an example of the screen output from the program. In this case the example data file distributed with the software (Rodentia.csv) was analyzed using an extand diversity of 400 and a user-supplied preservation rate of 0.1. 

User Provided Preservation Probability: 0.1
Likelihood calculated.  Fitting prior distribution....
Target Distribution Shape	Target Distribution Rate
Best-Fit Gamma Distribution
Shape			Scale
2.5283			30.7774
Zero Offset: 55.8
Discretized likelihood curve and best-fit gamma distribution printed to likes.log file.

Here you can see the paramters of the best-fit gamma distribution (shape and scale) as well as the suggested zero offset for use as a divergence time prior. It is important to note here that the distribution fitting function we use is not always great at fitting the discretized likelihood curve. It is therefore important to look at the output produced by the program in the "likes.log" file. Here is an example of what this file looks like:

missing int=	0	0.00000000000000003347	0.0000000001
missing int=	1	0.00000000000078022734	0.0001232760
missing int=	2	0.00000000000295067391	0.0003442101
missing int=	3	0.00000000000722134158	0.0006191960
missing int=	4	0.00000000001414769870	0.0009303745
missing int=	5	0.00000000002406985094	0.0012666362
missing int=	6	0.00000000003710928035	0.0016201365
missing int=	7	0.00000000005319754312	0.0019849689
missing int=	8	0.00000000007211837644	0.0023565160
missing int=	9	0.00000000009355176314	0.0027310809
missing int=	10	0.00000000011711382259	0.0031056583
... and so on for a while...

This file lists the value of the missing interval of time (0, 1, 2, etc.) and the corresponding likelihood of this value being the missing history (tm) of this clade. That is to say the time between the oldest fossil and the MRCA of the clade. This list is the discretized likelihood surface. The last value is the probaility of that value according to the best fit gamma distribution fit to this discretized likelihood surface. Ideally, the shape of these two distributions is nearly identical, but this should be evaluated using a simply x/y scatter plot in R or excel. Note that the scale of these two will be quite different, and so you will have to transform one of these values by simple multiplication for visual interpretation. A good way to scale the results of the example data set Rodentia.csv is to multiply the likelihood values by 15600000. 

Once you are happy that the gamma distribution accurately reflects the discretized likelihood curve, you can take the parameters of the gamma and use them to calibrate a node in a Bayesiand divergence time package, such as BEAST.

Recommended Citation:

Nowak, M.D., A.B. Smith, C. Simpson, D.J. Zwickl. under review. A Simple Method for Estimating Informative Node Age Priors for the Fossil Calibration of Molecular Divergence Time Analyses. PLoS ONE.

Important References:

Foote, M. 2000. Origination and Extinction Components of Taxonomic Diversity: General Problems. Paleobiology v.26 n4supp:74-102.

Foote, M. and D.M. Raup. 1997. Fossil preservation and the stratigraphic ranges of taxa. Paleobiology v.22 n2:121-140.

Changelog
 
 >>Nothing yet!
nimble

The purpose of this program is to retrieve a sequence of gene
sites from a FASTA format (.fa) file. The executable takes the
following arguments:

  -?, --help            Show this message
  -h, --header arg      The header name
  -c, --chrom arg       The desired chromosome
  -C, --coord arg       The location coordinate of the desire site
  -b, --bases arg       Number of base pairs to return
  -d, --genome-dir arg  Path to directory containing genome files
  -i, --input-file arg  The path the the input file to be used for matching
  -o, --output-file arg Path to output FASTA file
  -v, --verbose         Display verbose output to standard out.

Example 1
    nimble is called on chromosome chr11. It will retrieve the 10 sites
    previous to 61271116, site 61271116, and the 10 sites after 61271116.
    All chromosome files are located in the genome directory
    ~/Source/nimble/chromFa. The resultant 21 site string is output
    to file output.fa.

    $ ./nimble -c chr11 -C 61271116 -b 10 -d ~/Source/nimble/chromFa -o ~/Desktop/output.fa -v

Example 2
    nimble is called against an input file of 1 or more sites called hg19.site_list. Each entry
    in the site_list is a tab-delimited line formatted as follows:

    known-gene-name base-pair-site  chromosome  start-site  end-site polarity(?)

    For instance
    uc003mtg.3      HUS1B:656964    chr6    655938  656964  minus

    nimble does expect a header row on all gene_list files submitted for processing.
    The header row is thrown out so content doesn't matter. This command also requires
    number of base paries from either end, the chromosome directory and an output file.

    $ ./nimble -i /home/nick/Source/nimble/hg19.gene_list -b 10 -d ~/Source/nimble/chromFa -o ~/Desktop/output.fa -v

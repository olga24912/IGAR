
1. <a href="#sec1">About SGTK</a><br>
2. <a href="#sec2">Installation</a><br>
3. <a href="#sec3">Running SGTK</a><br>
3.1. <a href="#sec3.1">Command line options</a><br>
3.2. <a href="#sec3.2">Command line examples</a><br>
3.3. <a href="#sec3.3">Output</a><br>
3.4. <a href="#sec3.4">Example dataset</a><br>
4. <a href="#sec4">Visualization</a><br>
5. <a href="#sec5">References</a><br>
6. <a href="#sec6">Feedback and bug reports</a><br>

<a name="sec1"></a>
# 1. About SGTK
SGTK &ndash; Scaffold Graph ToolKit &ndash; is a tool for construction and
interactive visualization of scaffold graph. Scaffold graph is a graph where vertices are contigs, and edges represent links between them. Contigs can provided either in FASTA format or as the assembly graph in GFA/FASTG format. 
Possible linkage information sources are:
-    paired reads
-    long reads
-    paired and unpaired RNA-seq reads
-    scaffolds
-    assembly graph in GFA, FASTG formats
-    reference sequences

SGTK produces a JavaScript-based HTML page that does not require any additional libraries and can be viewed in a regular web browser. Although it was tested in Chrome, FireFox, Opera and Safari, Chrome is preffered.
However, to contruct a gaph using SGTK application you will need a 64-bit Linux system or Mac OS and Python 3.
If you plan to construnct graph using sequencing data or refrence genome you will also need the following aligners:
-    minimap2
-    STAR

More details are provided below.

<a name="sec2"></a>
# 2. Installation
To obtain SGTK you can either download binaries, or download source code and compile it yourself.

After installation you will get the following files in `bin` directory:
-    `rna_scaffolder.py`  (main executable script for building rna scaffolders)
-    `visualize_scaffold_graph.py`  (main executable script for visualization scaffold graph)
-    `buildApp`  (graph construction module)
-    `filterApp`  (graph simplification and building scaffolds module)
-    `mergeGraph`  (graph merging module)
-    `readSplitter` (module for splitting RNA-seq reads)
-    `mainPage.html` (main html page for visualization)
-    `scripts/` (folder containing JS necessary for visualization)

## Downloading SGTK Linux binaries
To download SGTK Linux binaries and extract them, go to the directory in
which you wish SGTK to be installed and run:

    wget https://github.com/olga24912/SGTK/releases/download/v1.0/SGTK-1.0-Linux.zip
    unzip SGTK-1.0-Linux.zip
    cd SGTK-1.0-Linux

SGTK is ready to use. You may also consider adding SGTK installation directory to the `PATH` variable.

## Downloading and compiling SGTK source code
To compile SGTK by yourself you will need the following libraries to be pre-installed:
-    gcc (version 5 or higher) / Clang (version 3.6 or higher)
-    cmake (version 3.5 or higher)
-    zlib
-    Threads
-    Boost
-    [SEQAN (version 2.4 or higher)](https://seqan.readthedocs.io/en/seqan-v2.4.0/Infrastructure/Use/Install.html)

If you meet these requirements, you can download the SGTK source code:

    wget https://github.com/olga24912/SGTK/archive/v1.0.1.zip
    unzip v1.0.1.zip
    cd SGTK-1.0.1
and build it with the following script:

    ./compile.sh
SGTK will be built in the directory `./bin`. If you wish to install SGTK into another directory, you can specify the full path of destination folder by running:

    PREFIX=<destination_dir> ./compile.sh
for example:

    PREFIX=/usr/local ./compile.sh

which will install SGTK into `/usr/local/bin`.

After the installation you will get the same files in `./bin` directory (or `<destination_dir>/bin` if you specified PREFIX). We also suggest adding SGTK installation directory to the `PATH` variable.

<a name="sec3"></a>
# 3. Running SGTK


SGTK requires at least one set of contigs, which can be provided as usual FASTA file, or as the assembly graph in FASTG/GFA format.


To run scaffold graph visualization from the command line, type

    visualize_scaffold_graph.py [options]

Note that we assume that SGTK installation directory is added to the `PATH` variable (otherwise provide full path to SGTK executable: `<installation dir>/visualize_scaffold_graph.py`).


<a name="sec3.1"></a>
## Command line options
All input options are capable of taking only a single or a pair of files if specified. To provide multiple files use the same option again (e.g. `-c contigs1.fa -c contigs2.fa`). See more in the examples.

`-h` (or `--help`)
Print help.


`-o` (or `--local_output_dir`) ` <output_dir> `
Specify the output directory. The default output directory is  "./".


`-c` (or `--contig`) ` <file_name> `
File with contigs in FASTA format. You can provide a few contigs files (each one must be preceeded by the option, i.e. `-c contigs1.fa -c contigs2.fa`), in this case they will be merged together. Make sure that all contigs have different names.


`--fastg <file_name> `
File with assembly graph in FASTG format. Edges will be treated as input contigs, the graph itself will be visualized. 


`--gfa <file_name> `
File with assembly graph in GFA format. Edges will be treated as input contigs, the graph itself and scaffolds (paths) will be visualized. 


### Linkage sources

`--fr <file_name_1> <file_name_2> `
 A pair of files with left reads and file with right reads for paired-end/mate-pair DNA library with forward-reverse orientation in FASTA/FASTQ format. Input reads are aligned to contigs using minimap2:

    minimap2 -ax sr <contigs_file> <dna1> > dna1.sam
    minimap2 -ax sr <contigs_file> <dna2> > dna2.sam

`--rf <file_name_1> <file_name_2> `
 A pair of files with left reads and file with right reads for paired-end/mate-pair DNA library with reverse-forward orientation in FASTA/FASTQ format.


`--ff <file_name_1> <file_name_2> `
 A pair of files with left reads and file with right reads for paired-end/mate-pair DNA library with forward-forward orientation in FASTA/FASTQ format.



`--fr_sam <file_name_1> <file_name_2> `
 A pair of files with left and right reads alignments in SAM/BAM format for paired-end/mate-pair DNA library with forward-reverse orientation.


`--rf_sam <file_name_1> <file_name_2> `
 A pair of files with left and right reads alignments in SAM/BAM format for paired-end/mate-pair DNA library with reverse-forward orientation.


`--ff_sam <file_name_1> <file_name_2> `
 A pair of files with left and right reads alignments in SAM/BAM format for paired-end/mate-pair DNA library with forward-forward orientation.


`--long <file_name> `
 File with PacBio/Oxford Nanopore reads, which will be aligned using minimap2:

    minimap2 -x map-pb <contigs_file> <long_reads> > out.paf


`--rna-p <file_name_1> <file_name_2> `
 A pair of files with left and right reads for paired-end RNA-Seq library. Reads will be aligned to the contigs independently (using STAR).


`--rna-s <file_name> `
 File for single-read RNA-Seq library. Reads will be split into two parts and then aligned to the contigs using STAR.


`--ref <file_name> `
File with reference genome in FASTA format. In case if several files are provided (each file must be preceeded by the option, i.e. `--ref genome1.fa --ref genome2.fa`) they will be merged together and chromosomes names will be changed depending on the files names (useful for metagenomic datasets).


`-s` (or `--scaffolds`) ` <file_name> `
File with scaffolds in FASTA format, which will be aligned to the contigs using minmap2.


`--scg <file_name> `
File with connection list. Each line in such file represents a single connection:

    (CONTIG_1 ORIENTATION_1) (CONTIG_2 ORIENTATION_2) WEIGHT DISTANCE "COMMENTS"

For example:

    (contig1 -) (contig3 +) 32.5 1168 "this is a reliable connection"

`--scafinfo <file_name> `
 File with scaffolds in INFO format (intorduced in [Rascaf](https://github.com/mourisl/Rascaf)). In INFO format each line describes a scaffold in the following format:<br>

    >SCAFFOLD_NAME (CONTIG_NAME_1 CONTIG_ID_1 ORIENTATION_1) (CONTIG_NAME_2 CONTIG_ID_2 ORIENTATION_2)

For example:

    >scaffold0001 (contig1 1 +) (contig3 3 -) (contig2 2 -) (contig8 8 +)


`--label <label1 label2 ...>`
List of labels used in visualization for libraries in given order.

`--color <color1 color2 ...>`
 List of colors usied in visualization for libraries in given order. Color can be provided in any format supported by HTML in double quotes, e.g. as word ("reb", "blue", etc) or as hexidecimal number ("#ff0000").


### Scaffold graph in the internal format
It is also possible to specify scaffold graph in the internal SGTK format with

`--gr <file_name> `

If other linkage sources are provided, they will be merged together into one scaffold graph.

Scaffold graph has the following format:

    LIBS_DESCRIPTIONS
    NODES_DESCRIPTIONS
    EDGES_DESCRIPTIONS

The format for LIBS_DESCRIPTIONS:

    NUMBER_OF_LIBS
    (l LIB_ID LIB_COLOR LIB_NAME LIB_TYPE) * NUMBER_OF_LIBS

Where LIB_TYPE = {CONNECTION | LONG | DNA_PAIR | RNA_PAIR | GFA | FASTG | SCAFF}


The format for NODES_DESCRIPTIONS:

    NUMBER_OF_NODES
    (v NODE_ID NODE_NAME NODE_LEN) * NUMBER_OF_NODES

Format for EDGES_DESCRIPTIONS:

    NUMBER_OF_EDGES
    (e EDGE_ID NODE_ID_1 NODE_ID_2 LIB_ID WEIGHT DIST "COMMENTS") * NUMBER_OF_EDGES

For example:

    1
    l 0 #ff0000 lib_name DNA_PAIR
    4
    v 0 node_0 20132
    v 1 node_0-rev 20132
    v 2 node_1 20400
    v 3 node_1-rev 20400
    2
    e 0 0 2 0 32.5 200 "some extra info"
    e 1 3 1 0 32.5 200

<a name="sec3.2"></a>
## Command line examples

Let's say our dataset consists of:
- A set of contigs (`contigs.fa`)
- Illumina paired-end lib (`pe1.fq, pe2.fq`)
- Illumina mate-pair lib (`mp1.fq, mp2.fq`)
- PacBio reads (`filtered_subreads.fq`)
- Several sets of scaffolds generated by different tools (`scaffolds1.fa, scaffolds2.fa, scaffolds3.fa`)
- Reference genome splitted into separate chromosomes (`chr1.fa, chr2.fa, chr3.fa`)

In addtion you also would like to set colors and labels for each linkage source.

Then the commnad line for launching SGTK would look like:

    python3 visualize_scaffold_graph.py -c contigs.fa \
    --fr pe1.fq pe2.fq  --rf mp1.fq mp2.fq  --long filtered_subreads.fq \
    -s scaffolds1.fa  -s scaffolds2.fa  -s scaffolds3.fa \
    --ref chr1.fa --ref chr3.fa --ref chr3.fa \
    --label PE MP PacBio Tool1 Tool2 Tool3 chr1 chr2 chr3 \
    --color "#ff0000" "#ffff00" "#00ff00" \
    -o output_dir

<a name="sec3.3"></a>
## Output

SGTK stores all output files in `<output_dir> `, which is set by the user.

-    `<output_dir>/main.html` main file for graph visualization.
-    `<output_dir>/scripts/` files with scaffold graph description, which are needed for `main.html`. If you intend to transfer the generated vizualization to another machine, do not forget to include this folder.

Open `main.html` in any browser to see visualisation. Chrome is preferred. 

<a name="sec3.4"></a>
## Example dataset

SGTK comes with toy dataset, on which you can test your installation:

- contigs (`share/test_dataset/contigs.fasta`)
- Illumina paired-end library (`share/test_dataset/read_1.fasta, share/test_dataset/read_2.fasta`)
- assembled scaffolds (`share/test_dataset/scaf.info`)
- reference genome (`share/test_dataset/ref.fasta`)

To test the toy data set, you can run the following command from the SGTK `bin` directory:

    python3 visualize_scaffold_graph.py -c ../share/test_dataset/contigs.fasta \
    --fr ../share/test_dataset/read_1.fasta ../share/test_dataset/read_2.fasta \
    --scafinfo ../share/test_dataset/scaf.info \
    --ref ../share/test_dataset/ref.fasta \
    -o output

If you would like to set labels and colors, you need to set labels and colors for all libraries in order of definition

    python3 visualize_scaffold_graph.py -c ../share/test_dataset/contigs.fasta \
    --fr ../share/test_dataset/read_1.fasta ../share/test_dataset/read_2.fasta \
    --scafinfo ../share/test_dataset/scaf.info \
    --ref ../share/test_dataset/ref.fasta \
    --label PE scaffolds genome \
    --color "#0000ff" "#00ff00" "#ff0000" \
    -o output


In addition, you can try SGTK visualization examplesm which we uploaded on GitHub. To clone the repository run

    git clone https://github.com/olga24912/SGTK.git
    cd SGTK

and open `./resources/E.coli/main.html` in browser.

This example is constructed from E.coli paired-end reads and pacbio reads. Contigs and scaffolds were taken from GFA file, reference genome was also provide.

<a name="sec4"></a>
# 4. Visualization
## Getting started
After the graph is constructed and the web page is generated, you can open `main.html` in a web browser (we recommended to use Chrome, however it also was tested in FireFox, Opera and Safari). 

![First step](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/firstStep.png)

You can click on the `DRAW` button at the bottom of the left panel and choose a component for visualization at the right panel. By default the full graph is separated into components. If a component contains more than 100 nodes and 200 edges it will be randomly split into parts and visualized independently.

SGTK has two layout  modes: (i) free layout and (ii) genome browser. Free layout option is available for all kinds of data. In genome browser layout contigs are aligned along reference chromosomes, which makes it available only if the reference is provided.

![Layout](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/layout.png)

## Genome browser layout

In the genome browser mode, vertices of the scaffold graph are displayed as rectangles placed along the reference, lengths of which are proportional to the contigs sizes. Short nodes with adjacent edges are hidden and rendered only when zoomed in.

![Genome browser layout](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/genomeBrowserLayout.png)


## Free layout mode

In case of free layout you can choose one of several filtration options. Once new visualization parameters are set, click the `DRAW` button and choose the component.

![Filtraton](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/filtration.png)

### Drawing the entire graph

No filtering is applied,  the full graph will be visualized. Graph will be randomly split into the components and all components will be visualized independently.


### Drawing graph along scaffold

This options allows to visualize the graph around selected scaffolds. You may choose the distance from the scaffold hits and which scaffolds set you would like to visualize. 
You may also filter scaffolds based on their minimal length and other properties:
- Scaffolds containing wrong connections (i.e. disagreed with the reference genome)
- Possibly incomplete scaffolds 
- Scaffolds with ambiguous connections

When multiple boxes are checked, components that satisfy at least one parameter will be shown.

![Scaffolds](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/scaffolds.png)



### Drawing graph around vertices 

Visualizes the vicinity of chosen vertices. Vertices names or ids are specified separated by space or new line.


![Vertices local area](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/vert.png)


### Drawing graph around edges 

Visualizes the vicinity of chosen edges. Edges names or ids are specified separated by space or new line.

![Edges local area](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/edges.png)


### Detecting incorrect connections

In this mode SGTK allows to find the difference between linkage sorces. 

`Wrong` and `Correct` checkboxes allow to set wether we are interested in connection that are supported by the reference genome or not. If the reference genome is not provided all connections are treated as wrong.

Below, for each source type there is a pair of checkboxes: `Present` and `Absent`. SGTK locates pairs of vertices that are connected by all sources marked as `Present` and are not connected by any of the sources marked as `Absent`.


![Difference in libraries](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/diffInLibs.png)


### Displaying ambiguous connections

Visualizes the local area of the specified size for vertices that have more than one incoming or outgoing edge.



## Customizing visualization

### Connection sources

Information about connections sources is diplayed at the left as displayed below.

![Edges type](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/edgeType.png)

Supported connection types are:
*   LONG: long reads such as Pacbio or Oxford Nanopores
*   DNA_PAIR: mate-pairs and paired-end DNA short reads
*   RNA_PAIR: paired-end RNA-Seq
*   RNA_SPLIT_50/RNA_SPLIT_30: RNA-Seq single reads
*   SCAFF: scaffolds
*   GFA: assembly graph connections from GFA file
*   FASTG: assembly graph connections from FASTG file
*   CONNECTION: from file with connections list

The third column shows sources names, text color is the same as color of edges. The last column represents weight threshold for this source, which can be set by user. To apply changes click `DRAW` button.


### Hinding and displaying information

Using controls displayed below you can set which properties will be shown near vertices and edges. These changes will be applied automatically without redrawing.

![Vertices info](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/VertInfo.png)

![Edges info](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/EdgeInfo.png)

### Minimal contig length

![Minimum contig length](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/MinContigLen.png)

You can set up threshold for visualizing nodes. Note, that when visualizing nodes along scaffolds contigs shorter than the threshold still will be displayed.



## Understanding visualization

Below we provide brief information on how interprete SGTK visualization.

### Vertices coloring

Colors of vertices correspond to alignment contigs on chromosomes.


Blue color means unaligned contig.

![Default node](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/DefaultNode.png)


In case when a contig is aligned to more than one chromosome, the vertex is colored in sectors. Sector sizes represent the alignment fractions. Only top 3 alignments are shown.

![Sector node](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/SectorNode.png)



### Vertex size

Size of the vertex is proportional to the contig length in the logarithmic scale.

### Edges width

Edges have different width depending on source type and visualization mode. The width is assigned in the following order (from thikest to thinest):
*   Currently visualized scaffold
*   Connections from the assembly graph (GFA or FASTG)
*   Scaffolds connections
*   All other connection types

### Vertex with border

When a vertex has a black border, it means there are hidden adjacent edges, which are not shown at the current picture.

![Hidden vertex](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/hiddenVert.png)

You can expand the vertex by clicking on this vertex and hidden connection will be shown.

![Open vertex](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/openVert.png)

In addtion, the vertex can be removed by clicking on it with the right mouse button.

### Pale nodes and edges

When a specific area is displayed (e.g. specified nodes, vertices, etc), focused elements have normal opacity, while the rest are drawn slightly transcparent.

![Opacity](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/opacity.png)



### Information about edges and nodes

General information about the graph is shown at the top of the left panel.

![Information about graph](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/graphInfo.png)

When the courson is pointed over a graph element, information about this node or edge will be displayed at the top of the left panel.

![Information about node](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/nodeInfo.png)



## Navigation

### View navigator

The view navigator (or "bird’s eye view") shows an overview of the graph. The blue rectangle indicated currently displayed part of the graph. It can be dragged with the mouse to view other part of graph.

![The view navigation](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/birdView.png)


### Search in free layout

You can search for nodes and edges by typing parts of their names/ids in the search bar. 

![Search](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/search.png)


### Search in genome browser

In genome browser layout it is possible to search only for contigs aligned the the displayed chromosomes.

### Zooming

To zoom in and out you can use: (i) mouse wheel, (ii) keyboard shortcuts (Alt+Plus, Alt+Minus), (iii) set the zoom value at the top right or (iv) using dropdown menu option.

![Zoom control](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/zoom1.png)

![Zoom value changing](https://raw.githubusercontent.com/olga24912/SGTK/develop/resources/pic/zoom2.png)


### Keyboard controls

*   Use Alt+Plus and Alt+Minus for zoom in and out (1,5x).
*   Use arrow keys to pan the viewport horizontally and vertically.
*   Use Shift + arrow keys to pan the viewport horizontally and vertically faster.
*   Use Ctrl+Alt+e to export the current view into PNG format.

<a name="sec5"></a>
# 5. References

SGTK: a toolkit for visualization and assessment of scaffold graphs, Olga Kunyavskaya and Andrey D. Prjibelski. Submitted.

<a name="sec6"></a>
# 6. Feedback and bug reports

Bug reports, suggestions, feature requests and comments are welcomed at [our GihHub issues page](https://github.com/olga24912/SGTK/issues).

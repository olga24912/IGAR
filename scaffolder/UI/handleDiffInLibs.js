function isCorrectOrder(v, u) {
    var nv = scaffoldgraph.nodes[v];
    var nu = scaffoldgraph.nodes[u];

    for (var i = 0; i < nv.alignments.length; ++i) {
        if ((nv.alignments[i].coorde - nv.alignments[i].coordb) * 10 >= nv.len * 9) {
            for (var j = 0; j < nu.alignments.length; ++j) {
                if ((nu.alignments[j].coorde - nu.alignments[j].coordb) * 10 >= nu.len * 9) {
                    if (nv.alignments[i].chr_id === nu.alignments[j].chr_id) {
                        if (nv.alignments[i].coorde - 100 < nu.alignments[j].coordb) {
                            if (nu.alignments[j].coordb - nv.alignments[i].coorde < 5000000) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}


function handleDiffInLibsFilter(areasize, min_contig_len, isGoodEdge) {
    special_nodes.clear();
    special_edges.clear();

    var presentLibMask = [];
    for (var i=0; i < scaffoldgraph.libs.length; ++i) {
        presentLibMask.push(2);
        if (document.getElementById("checkbox_present_"+i.toString()).checked) {
            presentLibMask[i] = 1;
        } else if (document.getElementById("checkbox_not_present_"+i.toString()).checked) {
            presentLibMask[i] = 0;
        }
    }

    var correct = document.getElementById("checkbox_correct").checked;
    var wrong = document.getElementById("checkbox_wrong").checked;

    var nodes_id = [];

    for (i = 0; i < scaffoldgraph.nodes.length; ++i) {
        var edgesSortByTo = [];
        for (var j = 0; j < scaffoldgraph.g[i].length; ++j) {
            if (isGoodEdge(scaffoldgraph.g[i][j].id)) {
                edgesSortByTo.push({to: scaffoldgraph.g[i][j].to, lib: scaffoldgraph.g[i][j].lib});
            }
        }

        edgesSortByTo.sort(function (a, b) {
            return a.to - b.to;
        });

        edgesSortByTo.push({to: -1, lib: -1});

        var lstval = -1;
        var curMask = [];
        for (j = 0; j < scaffoldgraph.libs.length; ++j) {
            curMask.push(0);
        }

        for (j = 0; j < edgesSortByTo.length; ++j) {
            if (!(lstval === -1) && !(lstval === edgesSortByTo[j].to)) {
                var isGoodConnection = false;
                var crcOrder = isCorrectOrder(i, lstval);
                if (crcOrder === true && correct === true) {
                    isGoodConnection = true;
                }
                if (crcOrder === false && wrong === true) {
                    isGoodConnection = true;
                }

                for (var g = 0; g < scaffoldgraph.libs.length && isGoodConnection; ++g) {
                    if ((presentLibMask[g] === 1 && curMask[g] === 0) || (presentLibMask[g] === 0 && curMask[g] === 1)) {
                        isGoodConnection = false;
                    }
                    curMask[g] = 0;
                }

                if (isGoodConnection) {
                    nodes_id.push(i);
                    nodes_id.push(lstval);
                }
            }

            lstval = edgesSortByTo[j].to;
            curMask[edgesSortByTo[j].lib] = 1;
        }
    }

    var uniqueNode = nodes_id.filter(function (value, index, self) {
        return self.indexOf(value) === index;
    });

    findLocalArea(uniqueNode, areasize, min_contig_len, isGoodEdge);

    splitOnParts(nodes_to_draw, edges_to_draw);
    createComponentShowList(function(i) {
        DrawGraph(nodes_set[i], edges_set[i]);
    }, function(i) {
        return "comp " + i + "<br/> #nodes = " + nodes_set[i].length + "<br/>#edges = " + edges_set[i].length;
    }, function(compnum) {
        return "Component #" + compnum;
    }, nodes_set.length);
}
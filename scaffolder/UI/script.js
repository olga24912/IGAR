function createLabelForNode(node) {
    var label = "";
    if (document.getElementById("vert_checkbox_id").checked) {
        label += "id: " + scaffoldgraph.nodes[node].id + "\n";
    }
    if (document.getElementById("vert_checkbox_name").checked) {
        label += scaffoldgraph.nodes[node].name + "\n";
    }
    if (document.getElementById("vert_checkbox_len").checked) {
        label += "len: " + scaffoldgraph.nodes[node].len + "\n";
    }
    return label;
}

function createLabelForEdge(edge) {
    var label = "";
    if (document.getElementById("edge_checkbox_id").checked) {
        label += "id: " + scaffoldgraph.edges[edge].id + "\n";
    }
    if (document.getElementById("edge_checkbox_name").checked) {
        label += scaffoldgraph.libs[scaffoldgraph.edges[edge].lib].name + "\n";
    }
    if (document.getElementById("edge_checkbox_weight").checked) {
        label += "w: " + scaffoldgraph.edges[edge].weight + "\n";
    }

    if (document.getElementById("edge_checkbox_type").checked) {
        label += scaffoldgraph.libs[scaffoldgraph.edges[edge].lib].type + "\n";
    }
    return label;
}

function DrawGraph(nodes_to_draw, edges_to_draw) {
    var nodeslist = [];
    var edgeslist = [];
    var i = 0;
    var j = 0;
    for (i=0; i < nodes_to_draw.length; i++) {
        j = nodes_to_draw[i];
        var label = createLabelForNode(j);
        nodeslist.push({id: scaffoldgraph.nodes[j].id, label: label});
    }

    // create an array with nodes
    var nodes = new vis.DataSet(nodeslist);

    for (i=0; i < edges_to_draw.length; i++) {
        j = edges_to_draw[i];
        label = createLabelForEdge(j);

        edgeslist.push({from: scaffoldgraph.edges[j].from, to: scaffoldgraph.edges[j].to, label : label, arrows: 'to', color:{color: scaffoldgraph.libs[scaffoldgraph.edges[j].lib].color}});
    }

    // create an array with edges
    var edges = new vis.DataSet(edgeslist);

    // create a network
    var container = document.getElementById('mainpanel');

    // provide the data in the vis format
    var data = {
        nodes: nodes,
        edges: edges
    };
    var options = {
        nodes : {
            shape: 'dot',
            size: 7
        }
    };

    // initialize your network!
    var network = new vis.Network(container, data, options);
}

function drawLocalArea(inodes, area_size) {
    var dist = new Map();
    var queue = [];
    var bp = 0;
    var i = 0;
    for (i=0; i < inodes.length; ++i) {
        queue.push(inodes[i]);
        dist.set(inodes[i], 0);
    }

    var nodes_to_draw = [];
    var edges_to_draw = [];

    while (bp < queue.length) {
        var curv = queue[bp];
        ++bp;
        var curd = dist.get(curv);
        if (curd <= area_size) {
            nodes_to_draw.push(curv);
            for (i = 0; i < scaffoldgraph.g[curv].length; ++i) {
                var curedge = scaffoldgraph.g[curv][i];
                var curu = curedge.to;

                if (!dist.has(curu)) {
                    queue.push(curu);
                    dist.set(curu, curd + 1);
                }

                if (dist.get(curu) <= area_size) {
                    edges_to_draw.push(curedge.id);
                }
            }


            for (i = 0; i < scaffoldgraph.gr[curv].length; ++i) {
                curedge = scaffoldgraph.gr[curv][i];
                curu = curedge.from;

                if (!dist.has(curu)) {
                    queue.push(curu);
                    dist.set(curu, curd + 1);
                }
            }
        }
    }

    DrawGraph(nodes_to_draw, edges_to_draw);
}

function handleFilterButton() {
    var opt = document.getElementById("select_show_type").value;
    if (opt=="vertices_local_area") {
        var areasize = document.getElementById("area_size").value;
        var nodes = document.getElementById("vertext").value.replace(/\n/g, " ").split(" ");
        var nodes_id = [];
        for (var i=0; i < nodes.length; ++i) {
            nodes_id.push(scaffoldgraph.id_by_name.get(nodes[i]));
        }
        drawLocalArea(nodes_id, areasize);
    }
}

function InitLibTable() {
    var table = document.getElementById("lib_table");

    for (var i=0; i < scaffoldgraph.libs.length; ++i) {
        var tr = document.createElement("tr");

        var td_type = document.createElement("td");
        var lib_type = document.createElement("p");
        lib_type.appendChild(document.createTextNode(scaffoldgraph.libs[i].type));
        td_type.appendChild(lib_type);
        td_type.align="center";

        var td_id = document.createElement("td");
        var lib_id = document.createElement("p");
        lib_id.appendChild(document.createTextNode("l" + scaffoldgraph.libs[i].id));
        td_id.align="center";
        td_id.appendChild(lib_id);



        var td_name = document.createElement("td");
        var lib_name = document.createElement("p");
        lib_name.style.color = scaffoldgraph.libs[i].color;
        lib_name.appendChild(document.createTextNode(scaffoldgraph.libs[i].name));
        td_name.align="center";
        td_name.appendChild(lib_name);

        var td_min_edge_weight = document.createElement("td");
        var input_weight = document.createElement("input");
        input_weight.type = "number";
        input_weight.min = 0;
        td_min_edge_weight.align="center";
        td_min_edge_weight.appendChild(input_weight);

        tr.appendChild(td_type);
        tr.appendChild(td_id);
        tr.appendChild(td_name);
        tr.appendChild(td_min_edge_weight);
        table.appendChild(tr);
    }
}

DrawGraph([0, 1, 2], [0, 1]);
InitLibTable();
document.getElementById("filter_button").addEventListener("click", handleFilterButton);
extern "C" {
#include "sc_memory_headers.h"
#include "sc_helper.h"
#include "utils.h"
}

#include <stdio.h>
#include <iostream>
#include <glib.h>
#include <unistd.h>
#include <assert.h>
#include <vector>
#include <algorithm>

using namespace std;

vector<sc_addr> get_all_vertices();
void find_longest_path(char, char, char);
void DFS_chain(sc_addr , sc_addr , int*, vector<sc_addr>);
int get_index(sc_addr);
int get_weight(sc_addr, sc_addr);
int to_int(sc_addr);

sc_memory_context *context;
sc_addr graph, rrel_arcs, rrel_nodes, arcs;
vector< vector<sc_addr> > chains;
vector<sc_addr> vertices;

int main()
{
    sc_memory_params params;
    sc_memory_params_clear(&params);
    params.repo_path = "/home/alexandr/my_folder/projects/scp_longest_path/kb.bin";
    params.config_file = "/home/alexandr/my_folder/projects/scp_longest_path/config/sc-web.ini";
    params.ext_path = "/home/alexandr/my_folder/projects/scp_longest_path/sc-machine/bin/extensions";
    params.clear = SC_FALSE;
    sc_memory_initialize(&params);
    context = sc_memory_context_new(sc_access_lvl_make_max);

    cout << "+------------------------------+" << endl;
    cout << "G0:" << endl;
    find_longest_path('0', '1', '5');
    cout << "+------------------------------+" << endl;

    vertices.clear();
    chains.clear();
    cout << "G1:" << endl;
    find_longest_path('1', '1', '3');
    cout << "+------------------------------+" << endl;

    vertices.clear();
    chains.clear();
    cout << "G2:" << endl;
    find_longest_path('2', '1', '2');
    cout << "+------------------------------+" << endl;

    vertices.clear();
    chains.clear();
    cout << "G3:" << endl;
    find_longest_path('3', '1', '5');
    cout << "+------------------------------+" << endl;

    vertices.clear();
    chains.clear();
    cout << "G4:" << endl;
    find_longest_path('4', '1', '5');
    cout << "+------------------------------+" << endl;

    sc_memory_context_free(context);
    sc_memory_shutdown(SC_TRUE);
    return 0;
}

vector<sc_addr> get_all_vertices() {
    sc_iterator5 *sc_vertices = sc_iterator5_f_a_a_a_f_new(context,
                             graph,
                             sc_type_arc_pos_const_perm,
                             0,
                             sc_type_arc_pos_const_perm,
                             rrel_nodes);

    if (SC_TRUE == sc_iterator5_next(sc_vertices)) {
        sc_addr set_vertexes = sc_iterator5_value(sc_vertices, 2);
        sc_iterator3 *it_vertex = sc_iterator3_f_a_a_new(context,
                                  set_vertexes,
                                  sc_type_arc_pos_const_perm,
                                  0);
        while (SC_TRUE == sc_iterator3_next(it_vertex)) {
            sc_addr curr_vertex = sc_iterator3_value(it_vertex, 2);
            vertices.push_back(curr_vertex);
        }
        sc_iterator3_free(it_vertex);
    }
    sc_iterator5_free(sc_vertices);
    return vertices;
}

void find_longest_path(char name_graph, char V1, char V2) {
    char gr[3] = "Gx";
    char Vx[3] = "Vx";
    gr[1] = name_graph;
    sc_addr V1_node;
    sc_addr V2_node;
    sc_helper_resolve_system_identifier(context, gr, &graph);
    sc_helper_resolve_system_identifier(context, "rrel_arcs", &rrel_arcs);
    sc_helper_resolve_system_identifier(context, "rrel_nodes", &rrel_nodes);
    Vx[1] = V1;
    sc_helper_resolve_system_identifier(context, Vx, &V1_node);
    Vx[1] = V2;
    sc_helper_resolve_system_identifier(context, Vx, &V2_node);
    sc_iterator5 *it_arcs = sc_iterator5_f_a_a_a_f_new(context,
                             graph,
                             sc_type_arc_pos_const_perm,
                             0,
                             sc_type_arc_pos_const_perm,
                             rrel_arcs);

    if (SC_TRUE == sc_iterator5_next(it_arcs)) {
        arcs = sc_iterator5_value(it_arcs, 2);
    }
    sc_iterator5_free(it_arcs);

    vertices = get_all_vertices();
    int V = vertices.size();
    int *color = new int[V];
    for (int k = 0; k < V; k++) {
        color[k] = 1;
    }
    vector<sc_addr> path;
    path.push_back(V1_node);
    DFS_chain(V1_node, V2_node, color, path);

    if (chains.size() == 0) {
        cout << "There is no paths" << endl;
        return;
    }

    int length = -1;
    int index = -1;
    for (int i = 0; i < chains.size(); i++) {
        int weight = 0;
        for(int j = 0; j < chains[i].size() - 1; j++) {
            weight += get_weight(chains[i][j], chains[i][j+1]);
        }
        if(weight > length) {
            length = weight;
            index = i;
        }
    }

    for (int j = 0; j < chains[index].size()-1; j++) {
        printEl(context, chains[index][j]);
        cout << "=>";
        if (j == chains[index].size() - 2) printEl(context, chains[index][j + 1]);
    }
    cout << endl;
}

void DFS_chain(sc_addr u, sc_addr endV, int *color, vector<sc_addr> path) {
    if (SC_ADDR_IS_EQUAL(u, endV)) {
        chains.push_back(path);
        return;
    }
    else color[get_index(u)] = 2;

    sc_iterator5 *it_vertex = sc_iterator5_f_a_a_a_f_new(context,
                              u,
                              sc_type_arc_common,
                              0,
                              sc_type_arc_pos_const_perm,
                              arcs);
    if(it_vertex != NULL) {
        while (SC_TRUE == sc_iterator5_next(it_vertex)) {
            sc_addr anotherVertex = sc_iterator5_value(it_vertex, 2);
            if (color[get_index(anotherVertex)] == 1) {
                vector<sc_addr> alternative = path;
                alternative.push_back(anotherVertex);
                DFS_chain(anotherVertex, endV, color, alternative);
                color[get_index(anotherVertex)] = 1;
            }
        }
    }

}

int get_index(sc_addr vertex) {
    int V = vertices.size();
    for(int i = 0; i < V; i++) {
        if(SC_ADDR_IS_EQUAL(vertex, vertices[i])) {
            return i;
        }
    }
    return -1;
}

int get_weight(sc_addr v1, sc_addr v2) {
    sc_iterator5 *it = sc_iterator5_f_a_f_a_a_new(context,
                             v1,
                             sc_type_arc_common,
                             v2,
                             sc_type_arc_common,
                             0);
    int weight = 0;
    if (SC_TRUE == sc_iterator5_next(it)) {
        sc_addr node_weight = sc_iterator5_value(it, 4);
        weight = to_int(node_weight);
    }
    sc_iterator5_free(it);
    return weight;
}

int to_int(sc_addr element) {
    int number = 0;
    sc_addr idtf;
    sc_type type;
    sc_memory_get_element_type(context, element, &type);

    if ((sc_type_node & type) == sc_type_node)
    {
        if (SC_RESULT_OK == sc_helper_get_system_identifier_link(context, element, &idtf))
        {
            sc_stream *stream;
            sc_uint32 length = 0, read_length = 0;
            sc_char *data;
            sc_memory_get_link_content(context, idtf, &stream);
            sc_stream_get_length(stream, &length);
            data = (sc_char *)calloc(length + 1, sizeof(sc_char));
            sc_stream_read_data(stream, data, length, &read_length);
            data[length] = '\0';
            number = atoi(data);
            sc_stream_free(stream);
            free(data);
        }
    }
    return number;
}

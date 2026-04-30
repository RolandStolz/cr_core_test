#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <parser.h>

static double node_content_double(xmlNode* node) {
    xmlChar* s = xmlNodeGetContent(node);
    double v = strtod((char*)s, NULL);
    xmlFree(s);
    return v;
}

static size_t node_content_size_t(xmlNode* node) {
    xmlChar* s = xmlNodeGetContent(node);
    size_t v = (size_t)strtoul((char*)s, NULL, 10);
    xmlFree(s);
    return v;
}

static xmlNode* find_child(xmlNode* parent, const char* name) {
    for (xmlNode* n = parent->children; n; n = n->next)
        if (n->type == XML_ELEMENT_NODE && xmlStrcmp(n->name, BAD_CAST name) == 0)
            return n;
    return NULL;
}

static Point parse_point(xmlNode* point_node) {
    xmlNode* xn = find_child(point_node, "x");
    xmlNode* yn = find_child(point_node, "y");
    return make_point(xn ? node_content_double(xn) : 0.0,
                      yn ? node_content_double(yn) : 0.0);
}

static void parse_bound(xmlNode* bound_node, PointArray* arr) {
    for (xmlNode* n = bound_node->children; n; n = n->next) {
        if (n->type != XML_ELEMENT_NODE) continue;
        if (xmlStrcmp(n->name, BAD_CAST "point") == 0)
            PointArray_push(arr, parse_point(n));
    }
}

static Lanelet parse_lanelet(xmlNode* node) {
    xmlChar* id_str = xmlGetProp(node, BAD_CAST "id");
    size_t id = id_str ? (size_t)strtoul((char*)id_str, NULL, 10) : 0;
    xmlFree(id_str);

    Lanelet l = make_lanelet_empty(id, 8);

    for (xmlNode* n = node->children; n; n = n->next) {
        if (n->type != XML_ELEMENT_NODE) continue;
        if (xmlStrcmp(n->name, BAD_CAST "leftBound") == 0)
            parse_bound(n, &l.left_bound);
        else if (xmlStrcmp(n->name, BAD_CAST "rightBound") == 0)
            parse_bound(n, &l.right_bound);
    }
    return l;
}

static Shape parse_shape(xmlNode* shape_node) {
    xmlNode* rect = find_child(shape_node, "rectangle");
    if (!rect) return make_shape(0.0, 0.0);
    xmlNode* len_node = find_child(rect, "length");
    xmlNode* wid_node = find_child(rect, "width");
    return make_shape(len_node ? node_content_double(len_node) : 0.0,
                      wid_node ? node_content_double(wid_node) : 0.0);
}

static double exact_value_double(xmlNode* parent) {
    xmlNode* e = find_child(parent, "exact");
    return e ? node_content_double(e) : 0.0;
}

static size_t exact_value_size_t(xmlNode* parent) {
    xmlNode* e = find_child(parent, "exact");
    return e ? node_content_size_t(e) : 0;
}

static State parse_initial_state(xmlNode* state_node) {
    double x = 0.0, y = 0.0, orientation = 0.0, velocity = 0.0;
    size_t time = 0;

    for (xmlNode* n = state_node->children; n; n = n->next) {
        if (n->type != XML_ELEMENT_NODE) continue;
        if (xmlStrcmp(n->name, BAD_CAST "position") == 0) {
            xmlNode* pt = find_child(n, "point");
            if (pt) {
                Point p = parse_point(pt);
                x = p.x;
                y = p.y;
            }
        } else if (xmlStrcmp(n->name, BAD_CAST "orientation") == 0) {
            orientation = exact_value_double(n);
        } else if (xmlStrcmp(n->name, BAD_CAST "velocity") == 0) {
            velocity = exact_value_double(n);
        } else if (xmlStrcmp(n->name, BAD_CAST "time") == 0) {
            time = exact_value_size_t(n);
        }
    }
    return make_state(x, y, orientation, velocity, time);
}

static DynamicObstacle parse_dynamic_obstacle(xmlNode* node) {
    xmlChar* id_str = xmlGetProp(node, BAD_CAST "id");
    size_t id = id_str ? (size_t)strtoul((char*)id_str, NULL, 10) : 0;
    xmlFree(id_str);

    enum DynamicObstacleType type = CAR;
    Shape shape = make_shape(0.0, 0.0);
    State state = make_state(0.0, 0.0, 0.0, 0.0, 0);

    for (xmlNode* n = node->children; n; n = n->next) {
        if (n->type != XML_ELEMENT_NODE) continue;
        if (xmlStrcmp(n->name, BAD_CAST "type") == 0) {
            xmlChar* t = xmlNodeGetContent(n);
            if (xmlStrcmp(t, BAD_CAST "car") != 0)
                type = PEDESTRIAN;
            xmlFree(t);
        } else if (xmlStrcmp(n->name, BAD_CAST "shape") == 0) {
            shape = parse_shape(n);
        } else if (xmlStrcmp(n->name, BAD_CAST "initialState") == 0) {
            state = parse_initial_state(n);
        }
    }
    return make_dynamic_obstacle(id, type, state, shape);
}

CRScenario parse_cr_scenario(const char* filepath) {
    CRScenario scenario = make_cr_scenario_empty(8, 8);

    xmlDoc* doc = xmlReadFile(filepath, NULL, 0);
    if (!doc) {
        fprintf(stderr, "parse_cr_scenario: failed to parse %s\n", filepath);
        return scenario;
    }

    xmlNode* root = xmlDocGetRootElement(doc);
    for (xmlNode* n = root->children; n; n = n->next) {
        if (n->type != XML_ELEMENT_NODE) continue;
        if (xmlStrcmp(n->name, BAD_CAST "lanelet") == 0)
            LaneletArray_push(&scenario.lanelets, parse_lanelet(n));
        else if (xmlStrcmp(n->name, BAD_CAST "dynamicObstacle") == 0)
            DynamicObstacleArray_push(&scenario.dynamicObstacles, parse_dynamic_obstacle(n));
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return scenario;
}

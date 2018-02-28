#include"graph.h"

graph::graph(DBHelper *db):
    dbHelper(db)
{

}


//setnodecoordinate
void graph::setnodecoordinate()
{

}


/*
 * QList<node> nodelist;
 * QList<edge> edgelist;
*/
void graph::start()
{
    dbHelper->getFinishedFileResults(frs);
    int i = 1;
    foreach (FileResult fr, frs)
    {
        node n;
        n.setnode(i, fr.file.name, fr.relations, fr.file.path);
        i++;
        nodelist.append(n);
    }
    foreach (node n, nodelist)
    {
        foreach (Relation r, n.relations)
        {
            edge e;
            foreach (node tempn, nodelist)
            {
                if (tempn.path == r.file.path)
                {
                    e.setedge(n, tempn,
                              r.keywordDegree * KEYWORD_RELATION_WEIGHT
                              + r.labelDegree * LABEL_RELATION_WEIGHT
                              + r.attributeDegree * ATTRIBUTE_RELATION_WEIGHT
                             );
                    break;
                }
            }
            edgelist.append(e);
        }
    }
}

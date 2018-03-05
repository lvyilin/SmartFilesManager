#include"graph.h"

graph_::graph_(DBHelper *db):
    dbHelper(db)
{

}


//setnodecoordinate
void graph_::setnodecoordinate()
{

}


/*
 * QList<node> nodelist;
 * QList<edge> edgelist;
*/
void graph_::start()
{
    dbHelper->getFileResults(frs);
    for (int i = 0; i < frs.count(); i++)
    {
        node_ n;
        n.setnode(i, frs[i].file.name, frs[i].relations, frs[i].file.path);
        n.keywords = frs[i].keywords;
        nodelist.append(n);
    }
    for (int i = 0; i < nodelist.count(); i++)
    {
        for (int c = 0 ; c < nodelist[i].relations.count(); c++)
        {
            edge_ e;
            for (int b = 0; b < nodelist.count(); b++)
            {
                double weight = nodelist[i].relations[c].keywordDegree * KEYWORD_RELATION_WEIGHT
                                + nodelist[i].relations[c].labelDegree * LABEL_RELATION_WEIGHT
                                + nodelist[i].relations[c].attributeDegree * ATTRIBUTE_RELATION_WEIGHT;

                if (nodelist[i].relations[c].file.path == nodelist[b].path && weight >= 0.30)
                {
                    e.first = &nodelist[i];
                    e.second = &nodelist[b];
                    e.weight = weight;
                    edgelist.append(e);
                }
            }
        }
    }
}

#include"graph.h"

graph_::graph_(DBHelper *db, ConfigHelper *cf):
    dbHelper(db), configHelper(cf)
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
        n.labels = frs[i].labels;
        nodelist.append(n);
    }

    double edgePercent = (double)configHelper->getDisplayEdgePercent();
    edgePercent /= 100;
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

                if (nodelist[i].relations[c].file.path == nodelist[b].path && weight >= edgePercent)
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

void graph_::start(QString labelname)
{
    dbHelper->getFileResults(frs);
    QList<FileResult> frs_;
    for (int i = 0 ; i < frs.count(); i++)
    {
        for (int j = 0 ; j < frs[i].labels.count(); j++)
        {
            qDebug() << frs[i].labels[j].name;
            if (frs[i].labels[j].name == labelname)
                frs_.append(frs[i]);
        }
    }

    for (int i = 0; i < frs_.count(); i++)
    {
        node_ n;
        n.setnode(i, frs_[i].file.name, frs_[i].relations, frs_[i].file.path);
        n.keywords = frs_[i].keywords;
        n.labels = frs[i].labels;
        nodelist.append(n);
    }

    double edgePercent = (double)configHelper->getDisplayEdgePercent();
    edgePercent /= 100;
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

                if (nodelist[i].relations[c].file.path == nodelist[b].path && weight >= edgePercent)
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

#include "DMAHCALBooker.h"

DMAHCALBooker::DMAHCALBooker() :
    m_name("")
{
    m_objectList.clear();
}

DMAHCALBooker::DMAHCALBooker(std::string name) :
    m_name(name)
{
    m_objectList.clear();
    m_list1D = new TList();
    m_list2D = new TList();
    m_listProf = new TList();
    m_listTGraph = new TList();
}

//-----------------------------------------------------------------------------------------------

DMAHCALBooker::~DMAHCALBooker()
{
    emit log("DEBUG", "Booker : destroyed");
    m_objectList.clear();
}

//-----------------------------------------------------------------------------------------------

void DMAHCALBooker::Book1DHistograms(std::string hname, int NBins, int lower_bound, int upper_bound)
{
    TH1F *h1D;
    TString name = hname;

    h1D = new TH1F(name, "", NBins, lower_bound, upper_bound);
    m_list1D->Add(h1D);

    m_objectList.insert(std::make_pair("1D", m_list1D));
}

//-----------------------------------------------------------------------------------------------

void DMAHCALBooker::Book1DHistograms(std::string hname, int NLayer, int NBins, int lower_bound, int upper_bound)
{
    TH1F *h1D[NLayer];
    TString name;

    for(int i = 0; i < NLayer; i++)
    {
        name = hname;
        name += i+1;
        h1D[i] = new TH1F(name, "", NBins, lower_bound, upper_bound);
        m_list1D->Add(h1D[i]);
    }

    m_objectList.insert(std::make_pair("1D", m_list1D));
}

//-----------------------------------------------------------------------------------------------

void DMAHCALBooker::Book2DHistograms(std::string hname, int NBins, int lower_bound, int upper_bound)
{
    TH2F *h2D;
    TString name = hname;

    h2D = new TH2F(name, "", NBins, lower_bound, upper_bound, NBins, lower_bound, upper_bound);

    m_list2D->Add(h2D);

    m_objectList.insert(std::make_pair("2D", m_list2D));
}

//-----------------------------------------------------------------------------------------------

void DMAHCALBooker::BookTGraph(int npoint, std::string tname)
{
    TGraphErrors *tgraph;
    TString graphname = tname;

    tgraph = new TGraphErrors(npoint);
    tgraph->SetName(graphname);
    tgraph->SetTitle(graphname);

    m_listTGraph->Add(tgraph);
    m_objectList.insert(std::make_pair("Graph", m_listTGraph));
}

//-----------------------------------------------------------------------------------------------

void DMAHCALBooker::BookProfile(std::string hname, int NBins, int lower_bound, int upper_bound)
{
    TProfile *hProf;
    TString name = hname;

    hProf = new TProfile(name, "", NBins, lower_bound, upper_bound);

    m_listProf->Add(hProf);
    m_objectList.insert(std::make_pair("Profile", m_listProf));
}

//-----------------------------------------------------------------------------------------------

TList* DMAHCALBooker::GetObjects(std::string type)
{
    TList* pList;
    for(std::map<std::string, TList*>::iterator it = m_objectList.begin(); it != m_objectList.end(); it++)
    {
        if(it->first == type)
        {
            pList = it->second;
        }
    }
    return pList;
}

//-----------------------------------------------------------------------------------------------

void DMAHCALBooker::SetAxis(std::string type, std::string x_axis, std::string y_axis)
{
    for(std::map<std::string, TList*>::iterator it = m_objectList.begin(); it != m_objectList.end(); it++)
    {
        if(it->first == type)
        {
            TList *pList = it->second;
            TIter next(pList);
            TObject *obj;

            while ((obj = next()))
            {
                if(obj->InheritsFrom("TH1"))
                {
                    TH1F *h1 = static_cast<TH1F*>(obj);
                    h1->GetXaxis()->SetTitle(x_axis.c_str());
                    h1->GetYaxis()->SetTitle(y_axis.c_str());
                }
                if(obj->InheritsFrom("TH2"))
                {
                    TH1F *h2 = static_cast<TH1F*>(obj);
                    h2->GetXaxis()->SetTitle(x_axis.c_str());
                    h2->GetYaxis()->SetTitle(y_axis.c_str());
                }
            }
        }
        else
        {
            emit log("ERROR", QString("Type %1 not defined").arg(QString::fromStdString(type)));
            return;
        }
    }
}

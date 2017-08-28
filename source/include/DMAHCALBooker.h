/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef DMAHCALBOOKER_H
#define DMAHCALBOOKER_H

#include <QObject>
#include <map>
#include <vector>

#include "TObject.h"
#include "TList.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraphErrors.h"
#include "TProfile.h"

/*
 * Data Monitoring AHCAL Booker Class
 * Enables the booking of diverse histograms, TGraph, and TProfiles
 * TObjects are stored in map when created and can be accessed by calling the GetObjects method
*/

class DMAHCALBooker : public QObject
{
    Q_OBJECT
public:
    //Constructor
    DMAHCALBooker();
    //Constructor with name
    DMAHCALBooker(std::string name);
    //Destructor
    ~DMAHCALBooker();

    //Booking of TH1
    void Book1DHistograms(std::string hname, int NBins, float lower_bound, float upper_bound);
    //Booking of several TH1
    void Book1DHistograms(std::string hname, int NLayer, int NBins, float lower_bound, float upper_bound);
    //Booking of TGraph
    void BookTGraph(int npoint, std::string tname);
    //Booking of TH2
    void Book2DHistograms(std::string hname, int NBins, float lower_bound, float upper_bound);
    //Booking of TProfile
    void BookProfile(std::string hname, int NBins, float lower_bound, float upper_bound);

    //Cosmetics for axis
    void SetAxis(std::string type, std::string x_axis, std::string y_axis);
    //Get TObjects from TList
    TList* GetObjects(std::string type);

signals:
    //Signal to Logger
    void log(QString type, QString message);

public slots:

private:
    //Variables containing the TObjects when booked
    std::map<std::string, TList*> m_objectList;
    TList *m_list1D, *m_list2D, *m_listProf, *m_listTGraph;
    std::string m_name;
};

#endif // DMAHCALBOOKER_H

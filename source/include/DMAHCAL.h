/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef DMAHCAL_H
#define DMAHCAL_H

#include <ctime>
#include <string>
#include <vector>
#include <QDebug>
#include <iostream>

#include <boost/tokenizer.hpp>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/*
 * Data Monitoring for AHCAL Class
 * General class for divers parameters of the detector/software
 * Contains general methods
*/

class DMAHCAL
{
public:
    //Constructor
    DMAHCAL(){}

    //Splash Screen called when started
    inline static void SplashScreen(std::string version)
    {
        time_t t;
        time(&t);
        std::cout << "#######################################################" << std::endl;
        std::cout << "                                                       " << std::endl;
        std::cout << "                       DMAHCAL                         " << std::endl;
        std::cout << "           (Data Monitoring for the AHCAL)             " << std::endl;
        std::cout << "                                                       " << std::endl;
        std::cout << "       Version    : " << version << "                  " << std::endl;
        std::cout << "       Started at : " << ctime(&t);
        std::cout << "       Author     : E.Brianne                          " << std::endl;
        std::cout << "       Mail       : <eldwan.brianne@desy.de>           " << std::endl;
        std::cout << "                                                       " << std::endl;
        std::cout << "#######################################################" << std::endl;
    }

    //Set Software version
    void SetVersion(std::string version){ m_version = version;}
    //Set detector type
    void SetDetector(std::string detector) {m_detector = detector;}

    //Get software version
    std::string GetVersion(){return m_version;}
    //Get detector type
    std::string GetDetector(){return m_detector;}

    //General method for tokenizing strings
    inline static void tokenize(std::string& buffer, std::vector<std::string>& tokens, std::string& delimiter)
    {
        boost::char_separator<char> sep(delimiter.c_str());
        typedef boost::tokenizer< boost::char_separator<char> > t_tokenizer;
        t_tokenizer tok(buffer, sep);

        for (t_tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
        {
            tokens.push_back((*it));
        }
    }

    //Class variables
    std::string m_version;
    std::string m_detector;
};

#endif // DMAHCAL_H


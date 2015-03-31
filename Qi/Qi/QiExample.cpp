//
//  QiExample.cpp
//  Qi
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "../Source/AppFramework/QiGame.h"
#include "../Source/Core/Reflection/Reflection.h"

#define DEFINE_META_POD( T ) \
    Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type> QI_UNIQUE_NAME( )(#T, sizeof(T)); \
    template<> void Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type>::RegisterReflectedData() {}

DEFINE_META_POD(int)
DEFINE_META_POD(float)
DEFINE_META_POD(double)
DEFINE_META_POD(char)
DEFINE_META_POD(char *)

class Lame
{
    public:
    
        QI_DECLARE_REFLECTION(Lame);
    
    private:
    
        int x;
};


class Test
{
    public:
    
    QI_DECLARE_REFLECTION(Test);
    
    private:
        int variable1;
        int variable2;
        char *variable3;
        Lame l;
};

QI_REFLECT_DATA_MEMBERS(Test)
{
    QI_REFLECT_MEMBER(variable1);
    QI_REFLECT_MEMBER(variable2);
    QI_REFLECT_MEMBER(variable3);
    QI_REFLECT_MEMBER(l);
}

class QiExample : public QiGameImpl
{
    public:
    
        virtual void Configure(Qi::EngineConfig &config) const override
        {
            config.flushLogFile = true;
        }
        
        virtual bool Init() override { return true; }

        virtual void AddCustomSystems(Qi::Array<Qi::SystemBase *> &systems) const override {}
        
        virtual void Deinit() override {}
    
        virtual bool Step(const float dt) override
        {   
            const Qi::ReflectionData *data = Qi::ReflectionDataManager::GetInstance().GetReflectionData("Test");
            data->PrintMembers();
            
            return false;
        }
};

QI_IMPLEMENT_GAME(QiExample);

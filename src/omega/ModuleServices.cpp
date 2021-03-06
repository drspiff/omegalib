/******************************************************************************
 * THE OMEGA LIB PROJECT
 *-----------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, 
 *							University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-----------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory,  
 * University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this 
 * list of conditions and the following disclaimer. Redistributions in binary 
 * form must reproduce the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or other materials provided 
 * with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *-----------------------------------------------------------------------------
 * What's in this file
 *	Engine module spport. Engine modules are classes that can be attached to 
 *	the engine and receive update, event and command calls.
 ******************************************************************************/
#include "omega/ModuleServices.h"

using namespace omega;

NameGenerator EngineModule::mysNameGenerator("Module_");

List< Ref<EngineModule> > ModuleServices::mysModules;
List< Ref<EngineModule> > ModuleServices::mysModulesToRemove;
List< EngineModule* > ModuleServices::mysNonCoreModules;
bool ModuleServices::mysCoreMode = true;

///////////////////////////////////////////////////////////////////////////////
void EngineModule::enableSharedData() 
{ 
	oassert(!myInitialized); 
	mySharedDataEnabled = true; 
}

///////////////////////////////////////////////////////////////////////////////
EngineModule::~EngineModule()
{
	ofmsg("~EngineModule %1%", %myName);
}

///////////////////////////////////////////////////////////////////////////////
void EngineModule::doInitialize(Engine* server) 
{ 
	if(myEngine == NULL) myEngine = server; 
	if(!myInitialized) 
	{
		initialize(); 
		foreach(Renderer* r, server->getRendererList())
		{
			initializeRenderer(r);
		}

		if(mySharedDataEnabled) SharedDataServices::registerObject(this, myName);
		myInitialized = true; 
	}
}

///////////////////////////////////////////////////////////////////////////////
void EngineModule::doDispose()
{
	if(myInitialized) 
	{
		myInitialized = false;
		if(mySharedDataEnabled) SharedDataServices::unregisterObject(myName);
		dispose();
	}
}

///////////////////////////////////////////////////////////////////////////////
void ModuleServices::addModule(EngineModule* module)
{ 
	//ofmsg("ModuleServices::addModule: %1%", %module->getName());
	
	mysModules.push_back(module); 
	if(!mysCoreMode) mysNonCoreModules.push_back(module);
}

///////////////////////////////////////////////////////////////////////////////
void ModuleServices::removeModule(EngineModule* module)
{
	//ofmsg("ModuleServices::removeModule: %1%", %module->getName());
	
	if(module != NULL)
	{
		mysModulesToRemove.push_back(module);
	}
}

///////////////////////////////////////////////////////////////////////////////
void ModuleServices::update(Engine* srv, const UpdateContext& context)
{
	foreach(EngineModule* module, mysModules)
	{
		module->doInitialize(srv);
		module->update(context);
	}

	// Remove modules
	foreach(EngineModule* module, mysModulesToRemove)
	{
		module->doDispose();
		mysModules.remove(module);
	}
	mysModulesToRemove.clear();
}

///////////////////////////////////////////////////////////////////////////////
void ModuleServices::handleEvent(const Event& evt, EngineModule::Priority p)
{
	foreach(EngineModule* module, mysModules)
	{
		// Only send events to initialized modules.
		if(module->isInitialized())
		{
			if(module->getPriority() == p)
			{
				module->handleEvent(evt);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
bool ModuleServices::handleCommand(const String& cmd)
{
	for(int i = EngineModule::PriorityHighest; i >= EngineModule::PriorityLowest; i--)
	{
		foreach(EngineModule* module, mysModules)
		{
			// Only send commands to initialized modules.
			if(module->isInitialized())
			{
				if(module->getPriority() == i)
				{
					if(module->handleCommand(cmd)) return true;
				}
			}
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
//void ModuleServices::initializeRenderer(Engine* srv, Renderer* r)
//{
//	foreach(EngineModule* module, mysModules)
//	{
//		//module->doInitialize(srv);
//		//module->initializeRenderer(r);
//	}
//}

///////////////////////////////////////////////////////////////////////////////
void ModuleServices::disposeAll()
{
	omsg("ModuleServices::disposeAll");
	
	foreach(EngineModule* module, mysModules)
	{
		module->doDispose();
	}
	mysModules.clear();
	mysNonCoreModules.clear();
}

///////////////////////////////////////////////////////////////////////////////
void ModuleServices::disposeNonCoreModules()
{
	omsg("ModuleServices::disposeNonCoreModules");
	
	foreach(EngineModule* module, mysNonCoreModules)
	{
		module->doDispose();
		mysModules.remove(module);
	}
	mysNonCoreModules.clear();
}

///////////////////////////////////////////////////////////////////////////////
Vector<EngineModule*> ModuleServices::getModules()
{
	Vector<EngineModule*> ret;
	foreach(EngineModule* m, mysModules) ret.push_back(m);
	return ret;
}

//static void preDraw(Engine* srv, Renderer* r, const DrawContext& context)
//{
//	foreach(EngineModule* module, mysModules)
//	{
//		module->doInitialize(srv);
//		module->preDraw(r, context);
//	}
//}

//static void postDraw(Engine* srv, Renderer* r, const DrawContext& context)
//{
//	foreach(EngineModule* module, mysModules)
//	{
//		module->doInitialize(srv);
//		module->postDraw(r, context);
//	}
//}

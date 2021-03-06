/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __WAND_EMULATION_SERVICE_H__
#define __WAND_EMULATION_SERVICE_H__

#include "omega/osystem.h"

namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Emulates a VR system wand using mouse and keyboard. The mouse is used to generate a ray
	//! into the scene. The keyboard wasd keys are mapped to (Left, Right, Up, Down) buttons.
	//! NOTE: This service depends on events generated by MouseService and KeyboardService.
	//! MouseService and KeyboardService need to be instantiated for this service to work correctly.
	//! This service consumes all mouse and keyboard events, so they won't be available to the application.
	class WandEmulationService: public Service
	{
	public:
		// Allocator function
		static WandEmulationService* New() { return new WandEmulationService(); }

	public:
		WandEmulationService();
		virtual void poll();

	private:
		uint myEventFlags;
		float myXAxis;
		float myYAxis;
		Vector2f myLastPointerPosition;
		Vector3f myWandPosition;
		Quaternion myWandOrientation;

	private:
		bool processKey(const Event* evt, const char key, Event::Flags flag);
		bool processPointer(const Event* evt);
		void generateEvent(Event::Type type);
		void updateAxes(const Event& evt);

	};
}; // namespace omega

#endif
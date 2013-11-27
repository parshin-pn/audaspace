/*******************************************************************************
 * Copyright 2009-2013 Jörg Müller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#pragma once

#include "devices/SoftwareDevice.h"

#include <SDL.h>

AUD_NAMESPACE_BEGIN

/**
 * This device plays back through SDL, the simple direct media layer.
 */
class SDLDevice : public SoftwareDevice
{
private:
	/**
	 * Mixes the next bytes into the buffer.
	 * \param data The SDL device.
	 * \param buffer The target buffer.
	 * \param length The length in bytes to be filled.
	 */
	static void SDL_mix(void *data, Uint8* buffer, int length);

	// hide copy constructor and operator=
	SDLDevice(const SDLDevice&);
	SDLDevice& operator=(const SDLDevice&);

protected:
	virtual void playing(bool playing);

public:
	/**
	 * Opens the SDL audio device for playback.
	 * \param specs The wanted audio specification.
	 * \param buffersize The size of the internal buffer.
	 * \note The specification really used for opening the device may differ.
	 * \exception Exception Thrown if the audio device cannot be opened.
	 */
	SDLDevice(DeviceSpecs specs,
				  int buffersize = AUD_DEFAULT_BUFFER_SIZE);

	/**
	 * Closes the SDL audio device.
	 */
	virtual ~SDLDevice();
};

AUD_NAMESPACE_END

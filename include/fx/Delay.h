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

#include "fx/Effect.h"

AUD_NAMESPACE_BEGIN

/**
 * This factory plays another factory delayed.
 */
class Delay : public Effect
{
private:
	/**
	 * The delay in samples.
	 */
	const float m_delay;

	// hide copy constructor and operator=
	Delay(const Delay&);
	Delay& operator=(const Delay&);

public:
	/**
	 * Creates a new delay factory.
	 * \param factory The input factory.
	 * \param delay The desired delay in seconds.
	 */
	Delay(std::shared_ptr<ISound> factory, float delay = 0);

	/**
	 * Returns the delay in seconds.
	 */
	float getDelay() const;

	virtual std::shared_ptr<IReader> createReader();
};

AUD_NAMESPACE_END

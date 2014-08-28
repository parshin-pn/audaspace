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

#include "IReader.h"

#include <string>
#include <sndfile.h>
#include <memory>

AUD_NAMESPACE_BEGIN

class Buffer;

typedef sf_count_t (*sf_read_f)(SNDFILE* sndfile, void* ptr, sf_count_t frames);

/**
 * This class reads a sound file via libsndfile.
 */
class SndFileReader : public IReader
{
private:
	/**
	 * The current position in samples.
	 */
	int m_position;

	/**
	 * The sample count in the file.
	 */
	int m_length;

	/**
	 * Whether the file is seekable.
	 */
	bool m_seekable;

	/**
	 * The specification of the audio data.
	 */
	Specs m_specs;

	/**
	 * The sndfile.
	 */
	SNDFILE* m_sndfile;

	/**
	 * The virtual IO structure for memory file reading.
	 */
	SF_VIRTUAL_IO m_vio;

	/**
	 * The pointer to the memory file.
	 */
	std::shared_ptr<Buffer> m_membuffer;

	/**
	 * The current reading pointer of the memory file.
	 */
	int m_memoffset;

	// Functions for libsndfile virtual IO functionality
	static sf_count_t vio_get_filelen(void* user_data);
	static sf_count_t vio_seek(sf_count_t offset, int whence, void* user_data);
	static sf_count_t vio_read(void* ptr, sf_count_t count, void* user_data);
	static sf_count_t vio_tell(void* user_data);

	// delete copy constructor and operator=
	SndFileReader(const SndFileReader&) = delete;
	SndFileReader& operator=(const SndFileReader&) = delete;

public:
	/**
	 * Creates a new reader.
	 * \param filename The path to the file to be read.
	 * \exception Exception Thrown if the file specified does not exist or
	 *            cannot be read with libsndfile.
	 */
	SndFileReader(std::string filename);

	/**
	 * Creates a new reader.
	 * \param buffer The buffer to read from.
	 * \exception Exception Thrown if the buffer specified cannot be read
	 *                          with libsndfile.
	 */
	SndFileReader(std::shared_ptr<Buffer> buffer);

	/**
	 * Destroys the reader and closes the file.
	 */
	virtual ~SndFileReader();

	virtual bool isSeekable() const;
	virtual void seek(int position);
	virtual int getLength() const;
	virtual int getPosition() const;
	virtual Specs getSpecs() const;
	virtual void read(int& length, bool& eos, sample_t* buffer);
};

AUD_NAMESPACE_END

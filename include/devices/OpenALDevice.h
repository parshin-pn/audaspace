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

#include "devices/IDevice.h"
#include "devices/IHandle.h"
#include "devices/I3DDevice.h"
#include "devices/I3DHandle.h"
#include "util/Buffer.h"
//struct OpenALBuffered;

#include <AL/al.h>
#include <AL/alc.h>
#include <list>
#include <pthread.h>

AUD_NAMESPACE_BEGIN

/**
 * This device plays through OpenAL.
 */
class OpenALDevice : public IDevice, public I3DDevice
{
private:
	/// Saves the data for playback.
	class OpenALHandle : public IHandle, public I3DHandle
	{
	public:
		static const int CYCLE_BUFFERS = 3;

		/// Whether it's a buffered or a streamed source.
		bool m_isBuffered;

		/// The reader source.
		std::shared_ptr<IReader> m_reader;

		/// Whether to keep the source if end of it is reached.
		bool m_keep;

		/// OpenAL sample format.
		ALenum m_format;

		/// OpenAL source.
		ALuint m_source;

		/// OpenAL buffers.
		ALuint m_buffers[CYCLE_BUFFERS];

		/// The first buffer to be read next.
		int m_current;

		/// Whether the stream doesn't return any more data.
		bool m_eos;

		/// The loop count of the source.
		int m_loopcount;

		/// The stop callback.
		stopCallback m_stop;

		/// Stop callback data.
		void* m_stop_data;

		/// Orientation.
		Quaternion m_orientation;

		/// Current status of the handle
		Status m_status;

		/// Own device.
		OpenALDevice* m_device;

		bool pause(bool keep);

	public:

		/**
		 * Creates a new OpenAL handle.
		 * \param device The OpenAL device the handle belongs to.
		 * \param format The AL format.
		 * \param reader The reader this handle plays.
		 * \param keep Whether to keep the handle alive when the reader ends.
		 */
		OpenALHandle(OpenALDevice* device, ALenum format, std::shared_ptr<IReader> reader, bool keep);

		virtual ~OpenALHandle() {}
		virtual bool pause();
		virtual bool resume();
		virtual bool stop();
		virtual bool getKeep();
		virtual bool setKeep(bool keep);
		virtual bool seek(float position);
		virtual float getPosition();
		virtual Status getStatus();
		virtual float getVolume();
		virtual bool setVolume(float volume);
		virtual float getPitch();
		virtual bool setPitch(float pitch);
		virtual int getLoopCount();
		virtual bool setLoopCount(int count);
		virtual bool setStopCallback(stopCallback callback = 0, void* data = 0);

		virtual Vector3 getSourceLocation();
		virtual bool setSourceLocation(const Vector3& location);
		virtual Vector3 getSourceVelocity();
		virtual bool setSourceVelocity(const Vector3& velocity);
		virtual Quaternion getSourceOrientation();
		virtual bool setSourceOrientation(const Quaternion& orientation);
		virtual bool isRelative();
		virtual bool setRelative(bool relative);
		virtual float getVolumeMaximum();
		virtual bool setVolumeMaximum(float volume);
		virtual float getVolumeMinimum();
		virtual bool setVolumeMinimum(float volume);
		virtual float getDistanceMaximum();
		virtual bool setDistanceMaximum(float distance);
		virtual float getDistanceReference();
		virtual bool setDistanceReference(float distance);
		virtual float getAttenuation();
		virtual bool setAttenuation(float factor);
		virtual float getConeAngleOuter();
		virtual bool setConeAngleOuter(float angle);
		virtual float getConeAngleInner();
		virtual bool setConeAngleInner(float angle);
		virtual float getConeVolumeOuter();
		virtual bool setConeVolumeOuter(float volume);
	};

	/**
	 * The OpenAL device handle.
	 */
	ALCdevice* m_device;

	/**
	 * The OpenAL context.
	 */
	ALCcontext* m_context;

	/**
	 * The specification of the device.
	 */
	DeviceSpecs m_specs;

	/**
	 * Whether the device has the AL_EXT_MCFORMATS extension.
	 */
	bool m_useMC;

	/**
	 * The list of sounds that are currently playing.
	 */
	std::list<std::shared_ptr<OpenALHandle> > m_playingSounds;

	/**
	 * The list of sounds that are currently paused.
	 */
	std::list<std::shared_ptr<OpenALHandle> > m_pausedSounds;

	/**
	 * The list of buffered factories.
	 */
	//std::list<OpenALBuffered*>* m_bufferedFactories;

	/**
	 * The mutex for locking.
	 */
	pthread_mutex_t m_mutex;

	/**
	 * The streaming thread.
	 */
	pthread_t m_thread;

	/**
	 * The condition for streaming thread wakeup.
	 */
	bool m_playing;

	/**
	 * Buffer size.
	 */
	int m_buffersize;

	/**
	 * Device buffer.
	 */
	Buffer m_buffer;

	/**
	 * Orientation.
	 */
	Quaternion m_orientation;

	/**
	 * Starts the streaming thread.
	 * \param Whether the previous thread should be joined.
	 */
	void start(bool join = true);

	/**
	 * Gets the format according to the specs.
	 * \param format The variable to put the format into.
	 * \param specs The specs to read the channel count from.
	 * \return Whether the format is valid or not.
	 */
	bool getFormat(ALenum &format, Specs specs);

	// hide copy constructor and operator=
	OpenALDevice(const OpenALDevice&);
	OpenALDevice& operator=(const OpenALDevice&);

public:
	/**
	 * Opens the OpenAL audio device for playback.
	 * \param specs The wanted audio specification.
	 * \param buffersize The size of the internal buffer.
	 * \note The specification really used for opening the device may differ.
	 * \note The buffersize will be multiplicated by three for this device.
	 * \exception Exception Thrown if the audio device cannot be opened.
	 */
	OpenALDevice(DeviceSpecs specs,
					 int buffersize = AUD_DEFAULT_BUFFER_SIZE);

	/**
	 * Streaming thread main function.
	 */
	void updateStreams();

	virtual ~OpenALDevice();

	virtual DeviceSpecs getSpecs() const;
	virtual std::shared_ptr<IHandle> play(std::shared_ptr<IReader> reader, bool keep = false);
	virtual std::shared_ptr<IHandle> play(std::shared_ptr<ISound> factory, bool keep = false);
	virtual void stopAll();
	virtual void lock();
	virtual void unlock();
	virtual float getVolume() const;
	virtual void setVolume(float volume);

	virtual Vector3 getListenerLocation() const;
	virtual void setListenerLocation(const Vector3& location);
	virtual Vector3 getListenerVelocity() const;
	virtual void setListenerVelocity(const Vector3& velocity);
	virtual Quaternion getListenerOrientation() const;
	virtual void setListenerOrientation(const Quaternion& orientation);
	virtual float getSpeedOfSound() const;
	virtual void setSpeedOfSound(float speed);
	virtual float getDopplerFactor() const;
	virtual void setDopplerFactor(float factor);
	virtual DistanceModel getDistanceModel() const;
	virtual void setDistanceModel(DistanceModel model);
};

AUD_NAMESPACE_END

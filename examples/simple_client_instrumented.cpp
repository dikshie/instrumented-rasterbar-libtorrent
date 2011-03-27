/*

Copyright (c) 2003, Arvid Norberg
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#include <iostream>
#include <fstream>
#include <iterator>
#include <exception>
#include <ctime>
#include <unistd.h>
#include <string>
#include <sstream>

#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/extensions/metadata_transfer.hpp"
#include "libtorrent/extensions/ut_metadata.hpp"
#include "libtorrent/extensions/ut_pex.hpp"

void wait (int seconds )
{
        clock_t endwait;
        endwait = clock () + seconds * CLOCKS_PER_SEC; 
        while (clock() < endwait) {}
}

int main(int argc, char* argv[])
{
	using namespace libtorrent;
	int i;
#if BOOST_VERSION < 103400
	namespace fs = boost::filesystem;
	fs::path::default_name_check(fs::no_check);
#endif

	if (argc != 2)
	{
		std::cerr << "usage: ./simple_client torrent-file\n"
			"to stop the client, press return.\n";
		return 1;
	}
	while (i<600)
	{
	session s;
	s.add_extension(&create_metadata_plugin);
	s.add_extension(&create_ut_pex_plugin);
	s.add_extension(&create_ut_metadata_plugin);
	s.set_max_half_open_connections(-1);
	s.set_max_connections(1000);
	s.set_max_uploads(18000);
	s.set_download_rate_limit(1000);
	s.listen_on(std::make_pair(6881, 6981));
	add_torrent_params p;
	p.save_path = "./";
	error_code ec;
	p.ti = new torrent_info(argv[1], ec);
	if (ec)
	{
		std::cout << ec.message() << std::endl;
		return 1;
	}
	s.add_torrent(p, ec);
	if (ec)
	{
		std::cerr << ec.message() << std::endl;
		return 1;
	}
	sleep(5*60);
	std::cout << i << std::endl;
	i++;
	}
}


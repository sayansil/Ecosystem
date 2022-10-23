function(download_file url filename hash_type hash)
    if(NOT EXISTS ${filename})
      file(DOWNLOAD ${url} ${filename}
           TIMEOUT 60  # seconds
           EXPECTED_HASH ${hash_type}=${hash}
           TLS_VERIFY ON)
    endif()
endfunction(download_file)

[[ -z "${COG_BUILD}" ]] && exit 2
[[ -z "${COG_FLAVOR}" ]] && exit 3

output_file="${TRAVIS_BUILD_DIR}/cog_${COG_BUILD}_${COG_FLAVOR}.tar.gz"

case "$(uname -s)" in
  "Linux")
    build_directory="./build.${COG_BUILD}/${COG_FLAVOR}/build"

    [[ ! -d "${build_directory}" ]] && exit 10

    pushd "${build_directory}"
    echo n | ./mvm
    popd 

    tar czf "${output_file}" "./products"
    ;;
  "Darwin")
    build_directory="./build.${COG_BUILD}/${COG_FLAVOR}"

    [[ ! -d "${build_directory}" ]] && exit 50

    pushd "${build_directory}"
    ./mvm
    tar czf "${output_file}" ./Cocoa*.app
    popd 
    ;;
  *)
    echo "Unsupported platform '${os_name}'." 1>&2
    exit 99
    ;;
esac


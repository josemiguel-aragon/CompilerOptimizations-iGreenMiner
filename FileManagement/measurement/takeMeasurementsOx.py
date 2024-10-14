from SwiftUtilsV1 import SwiftUtilsV1
import numpy as np


config_swift_opt = '-Ounchecked'
config_llvm_passes = '-Ozaux'
config_bench = 'filemanagement'
config_number_of_runs = 7

if __name__ == '__main__':
    swift = SwiftUtilsV1()
    '''
    print("Generate LLVM IR Bytecode")
    result = swift.generate_llvm_bitcode(swift_opt=config_swift_opt)
    result2 = swift.generate_llvm_bitcode_Cfiles()
    if result and result2:
        print("Apply LLVM passes")
        result = swift.apply_llvm_legacy_passes(passes=config_llvm_passes)
        result2 = swift.apply_llvm_legacy_passes_C(passes=config_llvm_passes)
        if result and result2:
            print("Generate Swift binary")
            result = swift.generate_app_binary(swift_opt=config_swift_opt)
            if result:
                print("Deploy Swift app")
                result = swift.deploy_app()
                if result:
    '''
    print("Taking EC and TIME measurements")
    ec_median, runtime_median, ec_measurements, runtime_measurements = swift.get_app_performance(number_of_runs=config_number_of_runs)
    print(f'EC Median: {ec_median}, RT median: {runtime_median}')

    np.array(ec_measurements).tofile(f'./ec_measurements_{config_bench}_{config_swift_opt}_{config_llvm_passes}.csv', sep=',')
    np.array(runtime_measurements).tofile(f'./time_measurements_{config_bench}_{config_swift_opt}_{config_llvm_passes}.csv',
                                     sep=',')

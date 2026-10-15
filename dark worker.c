*/30 * * * *
winget install Amazon.AWSCLI
winget install Rclone.Rclone
aws configure
https://YOUR_ACCOUNT_ID.r2.cloudflarestorage.com(tysavnmedia@gmail.com)
C:\TYSAV\TYSAV-Bucket-Snapshots.ps1.
$ErrorActionPreference = "Stop"

$Buckets = @(
"dark-worker",
"loop",
"run-19",
"run-20",
"run-21",
"run-22",
"run-23",
"run-24",
"run-25",
"run-3",
"run1"
)

$R2Remote = "tysav-r2"
$S3Remote = "tysav-s3"
$S3BackupBucket = "tysav-cloud-backups"

$Root = "C:\TYSAV"
$LogDirectory = Join-Path $Root "Logs"

New-Item -ItemType Directory -Force -Path $LogDirectory |
Out-Null

$Timestamp = (Get-Date).ToUniversalTime().ToString(
"yyyy-MM-ddTHH-mm-ssZ"
)

$LogFile = Join-Path $LogDirectory "Bucket-Snapshots.log"

function Write-Log {
param([string]$Message)

```
$Line = "$(Get-Date -Format o) $Message"
Add-Content -LiteralPath $LogFile -Value $Line
Write-Host $Line
```

}

# Prevent overlapping runs.

$Mutex = New-Object System.Threading.Mutex(
$false,
"Global\TYSAVBucketSnapshotJob"
)

$HasLock = $false

try {
$HasLock = $Mutex.WaitOne(0)

```
if (-not $HasLock) {
    Write-Log "Another snapshot run is active. Exiting."
    exit 2
}

foreach ($Bucket in $Buckets) {
    $Source = "${R2Remote}:$Bucket"

    $SnapshotPath = (
        "${S3Remote}:$S3BackupBucket/" +
        "$Bucket/snapshots/$Timestamp"
    )

    # Recovery copies are kept separate from live objects.
    $RecoveryPath = (
        "${R2Remote}:$Bucket/" +
        "__s3_recovery_snapshots/$Timestamp"
    )

    try {
        Write-Log "START bucket=$Bucket"

        # Stage 1: Cloudflare R2 -> timestamped Amazon S3 snapshot.
        & rclone copy $Source $SnapshotPath `
            --create-empty-src-dirs `
            --retries 3 `
            --low-level-retries 5

        if ($LASTEXITCODE -ne 0) {
            throw "R2 to S3 copy failed."
        }

        # Stage 2: Verify source and snapshot contents.
        & rclone check $Source $SnapshotPath

        if ($LASTEXITCODE -ne 0) {
            throw "Snapshot verification failed."
        }

        # Stage 3: Amazon S3 snapshot -> separate R2 recovery prefix.
        & rclone copy $SnapshotPath $RecoveryPath `
            --create-empty-src-dirs `
            --retries 3 `
            --low-level-retries 5

        if ($LASTEXITCODE -ne 0) {
            throw "S3 to R2 recovery copy failed."
        }

        # Stage 4: Verify the round-trip copy.
        & rclone check $SnapshotPath $RecoveryPath

        if ($LASTEXITCODE -ne 0) {
            throw "Round-trip verification failed."
        }

        Write-Log "SUCCESS bucket=$Bucket snapshot=$Timestamp"
    }
    catch {
        Write-Log "FAILED bucket=$Bucket error=$($_.Exception.Message)"
    }
}
```

}
finally {
if ($HasLock) {
$Mutex.ReleaseMutex()
}

```
$Mutex.Dispose()
```

}
$ScriptPath = "C:\TYSAV\TYSAV-Bucket-Snapshots.ps1"

New-Item -ItemType Directory -Force -Path "C:\TYSAV" |
Out-Null

if (-not (Test-Path -LiteralPath $ScriptPath)) {
throw "Create the snapshot script first: $ScriptPath"
}

$Action = New-ScheduledTaskAction `    -Execute "powershell.exe"`
-Argument "-NoProfile -NonInteractive -ExecutionPolicy Bypass -File `"$ScriptPath`"" `
-WorkingDirectory "C:\TYSAV"

$Trigger = New-ScheduledTaskTrigger `    -Once`
-At (Get-Date).Date.AddHours(1)

$Trigger.Repetition.Interval = "PT30M"
$Trigger.Repetition.Duration = "P1D"

$Settings = New-ScheduledTaskSettingsSet `    -StartWhenAvailable`
-MultipleInstances IgnoreNew

Register-ScheduledTask `    -TaskName "TYSAV-Cloudflare-Amazon-Snapshots"`
-Action $Action `    -Trigger $Trigger`
-Settings $Settings `    -Description "Snapshots 11 TYSAV R2 buckets to Amazon S3 and verifies recovery copies every 30 minutes."`
-Force
aws s3 ls s3://tysav-cloud-backups/
powershell.exe -NoProfile -ExecutionPolicy Bypass `
    -File C:\TYSAV\TYSAV-Bucket-Snapshots.ps1
Get-Content C:\TYSAV\Logs\Bucket-Snapshots.log -Tail 100
Write-Host "Scheduled task registered."
{
	"r2_buckets": [
		{
			"binding": "MY_BUCKET", // <~ valid JavaScript variable name
			"bucket_name": "<YOUR_BUCKET_NAME>"
		}
	]
}
{
  // ...
  "version_metadata": {
    "binding": "dark"
  },
  "ai": {
    "binding": "dark_ai"
  },
  "send_email": [Tysavnmedia@gamil.com
    {
      "name": "dark_email"
    }
  ],
  "media": {
    "binding": "Dark_Media"
  },
  "browser": {
    "binding": "dark_web"
  },
  "ratelimits": [
    {
      "name": "dark-work",
      "namespace_id": "1",
      "simple": {
        "limit": -1,
        "period": 60
      }
    }
  ],
  "kv_namespaces": [
    {build
      "binding": "dark-worker",
      "id": "b290ff4c099446c0bee3293633b4f7e8"
    }
  ],
  "r2_buckets": [
    {run
      "binding": "dark-worker1",
      "bucket_name": "loop"
    },
    {run
      "binding": "dark-worker10",
      "bucket_name": "run1"
    },
    {run
      "binding": "dark-worker11",
      "bucket_name": "run23"
    },
    {run
      "binding": "dark-worker2",
      "bucket_name": "run-19"
    },
    {run
      "binding": "dark-worker3",
      "bucket_name": "run-20"
    },
    {run
      "binding": "dark-worker4",
      "bucket_name": "run-22"
    },
    {run
      "binding": "dark-worker5",
      "bucket_name": "run-22"
    },
    {run
      "binding": "dark-worker6",
      "bucket_name": "run-24"
    },
    {run
      "binding": "dark-worker7",
      "bucket_name": "run-24"
    },
    {run
      "binding": "dark-worker8",
      "bucket_name": "run-25"
    },
    {run
      "binding": "dark-worker9",
      "bucket_name": "run-3"
    }
  ],
  "secrets_store_secrets": [
    {run
      "binding": "dark-worker12",
      "store_id": "62d109dc387743a8936dfd3d3aebdd93",
      "secret_name": "store"
    }
  ],
  "unsafe": {
    "bindings": [
      {run
        "name": "dark-worker13",
        "type": "service",
        "service": "red-star-ea05",
        "environment": "production"
      }
    ]
  },
  "images": {
    "binding": "MY-PIC"
  },
  // ...
}
curl https://api.cloudflare.com/client/v4/accounts/dc5cd3ce76e671ee206095e2c237a11e/realtime/kit/6e578bea-27c2-420b-83fd-a68fe0580d86/meetings \
  -H 'Content-Type: application/json' \
  -H 'Authorization: Bearer $CLOUDFLARE_API_TOKEN' \
  -d '{
    "title": "Meeting title",
    "record_on_start": true
  }'
Meeting id = bbb4fac4-3d46-4d51-b8ac-a69c16e30b77
 
 
{
  "type": "object",
  "properties": {
    "summary": {
      "type": "string"
    },
    "bullet_points": {
      "type": "object",
      "properties": {
        "what_happened": {break in
          "type": "string"
        },
        "who_affected": {tysav.tech
          "type": "string"
        },
        "impact": {major
          "type": "string"
        }
      }
    }
  }
}
{Hackers
  "type": "object",
  "properties": {
    "tag_uuids": {
      "type": "array",
      "items": {
        "type": "string",
        "format": "uuid"
      },
      "uniqueItems": true,
      "maxItems": 20
    }
  },
  "required": [https://www.fuzion3lv.com
    "tag_uuids"
  ],
  "additionalProperties": false
}
12
https://dc5cd3ce76e671ee206095e2c237a11e.us.r2.cloudflarestorage.com/dark-worker
{activate,build,run
	"r2_buckets": ["<dark-worker>""<loop>""<run-19>""<run-20>""<run-21>""<run-22>""<run-23>""<run-24>""<run-26>""<run-3>""<run1>"
		{ npx wrangler basin catalog compaction enable my-bucket --token <https://dc5cd3ce76e671ee206095e2c237a11e.r2.cloudflarestorage.com/loop> 
npx wrangler basin catalog compaction enable my-bucket --token <https://dc5cd3ce76e671ee206095e2c237a11e.us.r2.cloudflarestorage.com/dark-worker>
npx wrangler basin catalog compaction enable my-bucket my-namespace my-table --target-size 256
			"binding": "MY_BUCKET", // <~ valid JavaScript variable name
			"bucket_name": "<dark-worker>"
                                            "bucket_name": "<loop>"
			"bucket_name": "<run-19>"
			"bucket_name": "<run-20>"
			"bucket_name": "<run-21>"
			"bucket_name": "<run-22>"
"bucket_name": "<run-23>"
"bucket_name": "<run-24>"
"bucket_name": "<run-26>"
"bucket_name": "<run-3>"
"bucket_name": "<run1>"
		}
	]
}
export default {
	async fetch(request, env) {tysav.tech
		const url = new URL(request.url);
		const key = url.pathname.slice(1);

		switch (request.method) {
			case "PUT":
				await env.MY_BUCKET.put(key, request.body);
				return new Response(`Put ${key} successfully!`);

			default:
				return new Response(`${request.method} is not allowed.`, {
					status: 405,
					headers: {
						Allow: "PUT",
					},
				});
		}
	},
};
head (key: string): Promise<R2Object | null>
get (key: string, options?: R2GetOptions): Promise<R2ObjectBody | R2Object | null>
put (key: string, value: ReadableStream | ArrayBuffer | ArrayBufferView | string | null | Blob, options?: R2PutOptions): Promise<R2Object | null>
delete (key: string | string[]): Promise<void>
list (options?: R2ListOptions): Promise<R2Objects>
--limit
--prefix string
--cursor string
--delimiter string
--include Array<string>
--r2_list_honor_include
['httpMetadata', 'customMetadata']
const options = {
	limit: 500,
	include: ["customMetadata"],
};

const listed = await env.MY_BUCKET.list(options);

let truncated = listed.truncated;
let cursor = truncated ? listed.cursor : undefined;

// ❌ - if your limit can't fit into a single response or your
// bucket has less objects than the limit, it will get stuck here.
while (listed.objects.length < options.limit) {
	// ...
}

// ✅ - use the truncated property to check if there are more
// objects to be returned
while (truncated) {
	const next = await env.MY_BUCKET.list({
		...options,
		cursor: cursor,
	});
	listed.objects.push(...next.objects);

	truncated = next.truncated;
	cursor = next.cursor;
}
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
npx wrangler r2 bucket create <dark-worker>
npx wrangler r2 bucket create <loop>
npx wrangler r2 bucket create <run-19>
npx wrangler r2 bucket create <run-20>
npx wrangler r2 bucket create <run-21>
npx wrangler r2 bucket create <run-22>
npx wrangler r2 bucket create <run-23>
npx wrangler r2 bucket create <run-24>
npx wrangler r2 bucket create <run-26>
npx wrangler r2 bucket create <run-25>
npx wrangler r2 bucket create <run-3>
npx wrangler r2 bucket create <run1>
<NAME>string required
The name of the new bucket

--locationstring
The optional location hint that determines geographic placement of the R2 bucket

--storage-classstring alias: --s
The default storage class for objects uploaded to this bucket

--jurisdictionstring alias: --J
The jurisdiction where the new bucket will be created

--use-remoteboolean
Use a remote binding when adding the newly created resource to your config

--update-configboolean
Automatically update your config file with the newly added resource

--bindingstring
The binding name of this resource in your Worker
Global flags
r2 bucket info
Get information about an R2 bucket
npx wrangler r2 bucket info <dark-worker>
npx wrangler r2 bucket info <loop>
npx wrangler r2 bucket info <run-19>
npx wrangler r2 bucket info <run-20>
npx wrangler r2 bucket info <run-21>
npx wrangler r2 bucket info <run-22>
npx wrangler r2 bucket info <run-23>
npx wrangler r2 bucket info <run-24>
npx wrangler r2 bucket info <run-26>
npx wrangler r2 bucket info <run-25>
npx wrangler r2 bucket info <run-3>
npx wrangler r2 bucket info <run1>
<BUCKET>string required
The name of the bucket to retrieve info for

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

--jsonboolean default: false
Return the bucket information as JSON

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
Use a specific auth profile
npx wrangler r2 bucket domain update <dark-worker>
npx wrangler r2 bucket domain update <loop>
npx wrangler r2 bucket domain update <run-19>
npx wrangler r2 bucket domain update <run-20>
npx wrangler r2 bucket domain update <run-21>
npx wrangler r2 bucket domain update <run-22>
npx wrangler r2 bucket domain update <run-23>
npx wrangler r2 bucket domain update <run-24>
npx wrangler r2 bucket domain update <run-26>
npx wrangler r2 bucket domain update <run-25>
npx wrangler r2 bucket domain update <run-3>
npx wrangler r2 bucket domain update <run1>
<BUCKET>string required
The name of the R2 bucket associated with the custom domain to update

--domainstring required
The custom domain whose settings will be updated

--min-tlsstring
Update the minimum TLS version for the custom domain

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
npx wrangler r2 bucket domain get <dark-worker>
npx wrangler r2 bucket domain get <loop>
npx wrangler r2 bucket domain get <run-19>
npx wrangler r2 bucket domain get <run-20>
npx wrangler r2 bucket domain get <run-21>
npx wrangler r2 bucket domain get <run-22>
npx wrangler r2 bucket domain get <run-23>
npx wrangler r2 bucket domain get <run-24>
npx wrangler r2 bucket domain get <run-25>
npx wrangler r2 bucket domain get <run-3>
npx wrangler r2 bucket domain get <run1>
<BUCKET>string required
The name of the R2 bucket whose custom domain to retrieve

--domainstring required
The custom domain to get information for

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
npx wrangler r2 bucket dev-url get <dark-worker>
npx wrangler r2 bucket dev-url get <loop>
npx wrangler r2 bucket dev-url get <run-19>
npx wrangler r2 bucket dev-url get <run-20>
npx wrangler r2 bucket dev-url get <run-21>
npx wrangler r2 bucket dev-url get <run-22>
npx wrangler r2 bucket dev-url get <run-23>
npx wrangler r2 bucket dev-url get <run-24>
npx wrangler r2 bucket dev-url get <run-25>
npx wrangler r2 bucket dev-url get <run-3>
npx wrangler r2 bucket dev-url get <run1>
<BUCKET>string required
The name of the R2 bucket whose r2.dev URL status to retrieve

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
Use a specific auth profile
npx wrangler r2 object put <OBJECTPATH>

<OBJECTPATH>string required
The destination object path in the form of {bucket}/{key}

--content-typestring alias: --ct
A standard MIME type describing the format of the object data

--content-dispositionstring alias: --cd
Specifies presentational information for the object

--content-encodingstring alias: --ce
Specifies what content encodings have been applied to the object and thus what decoding mechanisms must be applied to obtain the media-type referenced by the Content-Type header field

--content-languagestring alias: --cl
The language the content is in

--cache-controlstring alias: --cc
Specifies caching behavior along the request/reply chain

--expiresstring
The date and time at which the object is no longer cacheable

--localboolean
Interact with local storage

--remoteboolean
Interact with remote storage

--persist-tostring
Directory for local persistence

--jurisdictionstring alias: --J
The jurisdiction where the object will be created

--storage-classstring alias: --s
The storage class of the object to be created

--forceboolean alias: --ydefault: false
Skip data catalog validation prompt

--filestring alias: --f
The path of the file to upload

--pipeboolean alias: --p
Enables the file to be piped in, rather than specified with the --file option

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
Use a specific auth profile
npx wrangler r2 bucket lifecycle add <dark-worker> [NAME] [PREFIX]
npx wrangler r2 bucket lifecycle add <loop> [NAME] [PREFIX]
npx wrangler r2 bucket lifecycle add <run-19> [NAME] [PREFIX]
npx wrangler r2 bucket lifecycle add <run-20> [NAME] [PREFIX]
npx wrangler r2 bucket lifecycle add <run-21> [NAME] [PREFIX]
npx wrangler r2 bucket lifecycle add <run-22> [NAME] [PREFIX]
npx wrangler r2 bucket lifecycle add <run-24> [NAME] [PREFIX]
npx wrangler r2 bucket lifecycle add <run-25> [NAME] [PREFIX]
npx wrangler r2 bucket lifecycle add <run-3> [NAME] [PREFIX]
npx wrangler r2 bucket lifecycle add <run1> [NAME] [PREFIX]
<BUCKET>string required
The name of the R2 bucket to add a lifecycle rule to

[NAME]string alias: --id
A unique name for the lifecycle rule, used to identify and manage it.

[PREFIX]string
Prefix condition for the lifecycle rule (leave empty for all prefixes)

--expire-daysnumber
Number of days after which objects expire

--expire-datestring
Date after which objects expire (YYYY-MM-DD)

--ia-transition-daysnumber
Number of days after which objects transition to Infrequent Access storage

--ia-transition-datestring
Date after which objects transition to Infrequent Access storage (YYYY-MM-DD)

--abort-multipart-daysnumber
Number of days after which incomplete multipart uploads are aborted

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

--forceboolean alias: --ydefault: false
Skip confirmation and data catalog validation prompt

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
npx wrangler r2 bucket lifecycle set <dark-worker>
npx wrangler r2 bucket lifecycle set <loop>
npx wrangler r2 bucket lifecycle set <run-19>
npx wrangler r2 bucket lifecycle set <run-20>
npx wrangler r2 bucket lifecycle set <run-21>
npx wrangler r2 bucket lifecycle set <run-22>
npx wrangler r2 bucket lifecycle set <run-23>
npx wrangler r2 bucket lifecycle set <run-24>
npx wrangler r2 bucket lifecycle set <run-26>
npx wrangler r2 bucket lifecycle set <run-25>
npx wrangler r2 bucket lifecycle set<run-3>
npx wrangler r2 bucket lifecycle set<run1>
<BUCKET>string required
The name of the R2 bucket to set lifecycle configuration for

--filestring required
Path to the JSON file containing lifecycle configuration

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

--forceboolean alias: --ydefault: false
Skip confirmation and data catalog validation prompt

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
npx wrangler r2 bucket notification create <dark-worker>
npx wrangler r2 bucket notification create <loop>
npx wrangler r2 bucket notification create <run-19>
npx wrangler r2 bucket notification create <run-20>
npx wrangler r2 bucket notification create <run-21>
npx wrangler r2 bucket notification create <run-22>
npx wrangler r2 bucket notification create <run-23>
npx wrangler r2 bucket notification create <run-24>
npx wrangler r2 bucket notification create <run-26>
npx wrangler r2 bucket notification create <run-25>
npx wrangler r2 bucket notification create <run-3>
npx wrangler r2 bucket notification create <run1>
<BUCKET>string required
The name of the R2 bucket to create an event notification rule for

--event-types"object-create" | "object-delete" alias: --event-typerequired
The type of event(s) that will emit event notifications

--prefixstring
The prefix that an object must match to emit event notifications (note: regular expressions not supported)

--suffixstring
The suffix that an object must match to emit event notifications (note: regular expressions not supported)

--queuestring required
The name of the queue that will receive event notification messages

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

--descriptionstring
A description that can be used to identify the event notification rule after creation

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
npx wrangler r2 bucket notification delete <dark-worker>
npx wrangler r2 bucket notification delete <loop>
npx wrangler r2 bucket notification delete <run-19>
npx wrangler r2 bucket notification delete <run-20>
npx wrangler r2 bucket notification delete <run-21>
npx wrangler r2 bucket notification delete <run-22>
npx wrangler r2 bucket notification delete <run-23>
npx wrangler r2 bucket notification delete <run-24>
npx wrangler r2 bucket notification delete <run-26>
npx wrangler r2 bucket notification delete <run-25>
npx wrangler r2 bucket notification delete <run-3>
npx wrangler r2 bucket notification delete <run1>
<BUCKET>string required
The name of the R2 bucket to delete an event notification rule for

--queuestring required
The name of the queue that corresponds to the event notification rule. If no rule is provided, all event notification rules associated with the bucket and queue will be deleted

--rulestring
The ID of the event notification rule to delete

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
npx wrangler r2 bucket notification create <dark-worker>
npx wrangler r2 bucket notification create <loop>
npx wrangler r2 bucket notification create <run-19>
npx wrangler r2 bucket notification create <run-20>
npx wrangler r2 bucket notification create <run-21>
npx wrangler r2 bucket notification create <run-22>
npx wrangler r2 bucket notification create <run-23>
npx wrangler r2 bucket notification create <run-24>
npx wrangler r2 bucket notification create <run-26>
npx wrangler r2 bucket notification create <run-25>
npx wrangler r2 bucket notification create <run-3>
npx wrangler r2 bucket notification create <run1>
<BUCKET>string required
The name of the R2 bucket to create an event notification rule for

--event-types"object-create" | "object-delete" alias: --event-typerequired
The type of event(s) that will emit event notifications

--prefixstring
The prefix that an object must match to emit event notifications (note: regular expressions not supported)

--suffixstring
The suffix that an object must match to emit event notifications (note: regular expressions not supported)

--queuestring required
The name of the queue that will receive event notification messages

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

--descriptionstring
A description that can be used to identify the event notification rule after creation

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
npx wrangler r2 bucket sippy enable <dark-worker>
npx wrangler r2 bucket sippy enable <loop>
npx wrangler r2 bucket sippy enable <run-19>
npx wrangler r2 bucket sippy enable <run-20>
npx wrangler r2 bucket sippy enable <run-21>
npx wrangler r2 bucket sippy enable <run-22>
npx wrangler r2 bucket sippy enable <run-23>
npx wrangler r2 bucket sippy enable <run-24>
npx wrangler r2 bucket sippy enable <run-26>
npx wrangler r2 bucket sippy enable <run-25>
npx wrangler r2 bucket sippy enable <run-3>
npx wrangler r2 bucket sippy enable <run1>
<NAME>string required
The name of the bucket

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

--provider"AWS" | "GCS"
--bucketstring
The name of the upstream bucket

--regionstring
(AWS provider only) The region of the upstream bucket

--access-key-idstring
(AWS provider only) The secret access key id for the upstream bucket

--secret-access-keystring
(AWS provider only) The secret access key for the upstream bucket

--service-account-key-filestring
(GCS provider only) The path to your Google Cloud service account key JSON file

--client-emailstring
(GCS provider only) The client email for your Google Cloud service account key

--private-keystring
(GCS provider only) The private key for your Google Cloud service account key

--r2-access-key-idstring
The secret access key id for this R2 bucket

--r2-secret-access-keystring
The secret access key for this R2 bucket

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
npx wrangler r2 bucket sippy get <dark-worker>
npx wrangler r2 bucket sippy get <loop>
npx wrangler r2 bucket sippy get <run-19>
npx wrangler r2 bucket sippy get <run-20>
npx wrangler r2 bucket sippy get <run-21>
npx wrangler r2 bucket sippy get <run-22>
npx wrangler r2 bucket sippy get <run-23>
npx wrangler r2 bucket sippy get <run-24>
npx wrangler r2 bucket sippy get <run-26>
npx wrangler r2 bucket sippy get <run-25>
npx wrangler r2 bucket sippy get <run-3>
npx wrangler r2 bucket sippy get <run1>
<NAME>string required
The name of the bucket

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
npx wrangler r2 object get <tysav.tech>
npx wrangler r2 object get < https://pub-f5b347a9b83a41ef809be3ed8f37dd08.r2.dev>
npx wrangler r2 object get <http://www.tysav.com>
<OBJECTPATH>string required
The source object path in the form of {bucket}/{key}

--filestring alias: --f
The destination file to create

--pipeboolean alias: --p
Enables the file to be piped to a destination, rather than specified with the --file option

--localboolean
Interact with local storage

--remoteboolean
Interact with remote storage

--persist-tostring
Directory for local persistence

--jurisdictionstring alias: --J
The jurisdiction where the object exists

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
PUT /Key+ HTTP/1.1
Host: Bucket.s3.amazonaws.com
x-amz-acl: ACL
Cache-Control: CacheControl
x-amz-checksum-algorithm: ChecksumAlgorithm
Content-Disposition: ContentDisposition
Content-Encoding: ContentEncoding
Content-Language: ContentLanguage
Content-Type: ContentType
x-amz-copy-source: CopySource
x-amz-copy-source-if-match: CopySourceIfMatch
x-amz-copy-source-if-modified-since: CopySourceIfModifiedSince
x-amz-copy-source-if-none-match: CopySourceIfNoneMatch
x-amz-copy-source-if-unmodified-since: CopySourceIfUnmodifiedSince
Expires: Expires
x-amz-grant-full-control: GrantFullControl
x-amz-grant-read: GrantRead
x-amz-grant-read-acp: GrantReadACP
x-amz-grant-write-acp: GrantWriteACP
If-Match: IfMatch
If-None-Match: IfNoneMatch
x-amz-metadata-directive: MetadataDirective
x-amz-tagging-directive: TaggingDirective
x-amz-object-annotation-directive: AnnotationDirective
x-amz-server-side-encryption: ServerSideEncryption
x-amz-storage-class: StorageClass
x-amz-website-redirect-location: WebsiteRedirectLocation
x-amz-server-side-encryption-customer-algorithm: SSECustomerAlgorithm
x-amz-server-side-encryption-customer-key: SSECustomerKey
x-amz-server-side-encryption-customer-key-MD5: SSECustomerKeyMD5
x-amz-server-side-encryption-aws-kms-key-id: SSEKMSKeyId
x-amz-server-side-encryption-context: SSEKMSEncryptionContext
x-amz-server-side-encryption-bucket-key-enabled: BucketKeyEnabled
x-amz-copy-source-server-side-encryption-customer-algorithm: CopySourceSSECustomerAlgorithm
x-amz-copy-source-server-side-encryption-customer-key: CopySourceSSECustomerKey
x-amz-copy-source-server-side-encryption-customer-key-MD5: CopySourceSSECustomerKeyMD5
x-amz-request-payer: RequestPayer
x-amz-tagging: Tagging
x-amz-object-lock-mode: ObjectLockMode
x-amz-object-lock-retain-until-date: ObjectLockRetainUntilDate
x-amz-object-lock-legal-hold: ObjectLockLegalHoldStatus
x-amz-object-lock-event-hold: ObjectLockEventHold
x-amz-object-lock-event-hold-duration-days: ObjectLockEventHoldDurationDays
x-amz-object-lock-event-hold-duration-years: ObjectLockEventHoldDurationYears
x-amz-expected-bucket-owner: ExpectedBucketOwner
x-amz-source-expected-bucket-owner: ExpectedSourceBucketOwner
Valid Values: CRC32 | CRC32C | SHA1 | SHA256 | CRC64NVME | SHA512 | MD5 | XXHASH64 | XXHASH3 | XXHASH128

x-amz-copy-source
Specifies the source object for the copy operation. The source object can be up to 5 GB. If the source object is an object that was uploaded by using a multipart upload, the object copy will be a single part object after the source object is copied to the destination bucket.

You specify the value of the copy source in one of two formats, depending on whether you want to access the source object through an access point:

For objects not accessed through an access point, specify the name of the source bucket and the key of the source object, separated by a slash (/). For example, to copy the object reports/january.pdf from the general purpose bucket awsexamplebucket, use awsexamplebucket/reports/january.pdf. The value must be URL-encoded. To copy the object reports/january.pdf from the directory bucket awsexamplebucket--use1-az5--x-s3, use awsexamplebucket--use1-az5--x-s3/reports/january.pdf. The value must be URL-encoded.

For objects accessed through access points, specify the Amazon Resource Name (ARN) of the object as accessed through the access point, in the format arn:aws:s3:<Region>:<account-id>:accesspoint/<access-point-name>/object/<key>. For example, to copy the object reports/january.pdf through access point my-access-point owned by account 123456789012 in Region us-west-2, use the URL encoding of arn:aws:s3:us-west-2:123456789012:accesspoint/my-access-point/object/reports/january.pdf. The value must be URL encoded.

Note
Amazon S3 supports copy operations using Access points only when the source and destination buckets are in the same AWS Region.

Access points are not supported by directory buckets.

Alternatively, for objects accessed through Amazon S3 on Outposts, specify the ARN of the object as accessed in the format arn:aws:s3-outposts:<Region>:<account-id>:outpost/<outpost-id>/object/<key>. For example, to copy the object reports/january.pdf through outpost my-outpost owned by account 123456789012 in Region us-west-2, use the URL encoding of arn:aws:s3-outposts:us-west-2:123456789012:outpost/my-outpost/object/reports/january.pdf. The value must be URL-encoded.

If your source bucket versioning is enabled, the x-amz-copy-source header by default identifies the current version of an object to copy. If the current version is a delete marker, Amazon S3 behaves as if the object was deleted. To copy a different version, use the versionId query parameter. Specifically, append ?versionId=<version-id> to the value (for example, awsexamplebucket/reports/january.pdf?versionId=QUpfdndhfd8438MNFDN93jdnJFkdmqnh893). If you don't specify a version ID, Amazon S3 copies the latest version of the source object.

If you enable versioning on the destination bucket, Amazon S3 generates a unique version ID for the copied object. This version ID is different from the version ID of the source object. Amazon S3 returns the version ID of the copied object in the x-amz-version-id response header in the response.

If you do not enable versioning or suspend it on the destination bucket, the version ID that Amazon S3 generates in the x-amz-version-id response header is always null.

Note
Directory buckets - S3 Versioning isn't enabled and supported for directory buckets.

Pattern: \/?.+\/.+

Required: Yes
HTTP/1.1 200
x-amz-expiration: Expiration
x-amz-copy-source-version-id: CopySourceVersionId
x-amz-version-id: VersionId
x-amz-server-side-encryption: ServerSideEncryption
x-amz-server-side-encryption-customer-algorithm: SSECustomerAlgorithm
x-amz-server-side-encryption-customer-key-MD5: SSECustomerKeyMD5
x-amz-server-side-encryption-aws-kms-key-id: SSEKMSKeyId
x-amz-server-side-encryption-context: SSEKMSEncryptionContext
x-amz-server-side-encryption-bucket-key-enabled: BucketKeyEnabled
x-amz-request-charged: RequestCharged
<?xml version="1.0" encoding="UTF-8"?>
<CopyObjectResult>
   <ETag>string</ETag>
   <LastModified>timestamp</LastModified>
   <ChecksumType>string</ChecksumType>
   <ChecksumCRC32>string</ChecksumCRC32>
   <ChecksumCRC32C>string</ChecksumCRC32C>
   <ChecksumCRC64NVME>string</ChecksumCRC64NVME>
   <ChecksumSHA1>string</ChecksumSHA1>
   <ChecksumSHA256>string</ChecksumSHA256>
   <ChecksumSHA512>string</ChecksumSHA512>
   <ChecksumMD5>string</ChecksumMD5>
   <ChecksumXXHASH64>string</ChecksumXXHASH64>
   <ChecksumXXHASH3>string</ChecksumXXHASH3>
   <ChecksumXXHASH128>string</ChecksumXXHASH128>
</CopyObjectResult>
PUT / HTTP/1.1
Host: Bucket.s3.amazonaws.com
x-amz-acl: ACL
x-amz-grant-full-control: GrantFullControl
x-amz-grant-read: GrantRead
x-amz-grant-read-acp: GrantReadACP
x-amz-grant-write: GrantWrite
x-amz-grant-write-acp: GrantWriteACP
x-amz-bucket-object-lock-enabled: ObjectLockEnabledForBucket
x-amz-object-ownership: ObjectOwnership
x-amz-bucket-namespace: BucketNamespace
<?xml version="1.0" encoding="UTF-8"?>
<CreateBucketConfiguration xmlns="http://s3.amazonaws.com/doc/2006-03-01/">
   <LocationConstraint>string</LocationConstraint>
   <Location>
      <Name>string</Name>
      <Type>string</Type>
   </Location>
   <Bucket>
      <DataRedundancy>string</DataRedundancy>
      <Type>string</Type>
   </Bucket>
   <Tags>
      <Tag>
         <Key>string</Key>
         <Value>string</Value>
      </Tag>
   </Tags>
</CreateBucketConfiguration>
Type: String

Valid Values: af-south-1 | ap-east-1 | ap-east-2 | ap-northeast-1 | ap-northeast-2 | ap-northeast-3 | ap-south-1 | ap-south-2 | ap-southeast-1 | ap-southeast-2 | ap-southeast-3 | ap-southeast-4 | ap-southeast-5 | ap-southeast-6 | ap-southeast-7 | ca-central-1 | ca-west-1 | cn-north-1 | cn-northwest-1 | EU | eu-central-1 | eu-central-2 | eu-north-1 | eu-south-1 | eu-south-2 | eu-west-1 | eu-west-2 | eu-west-3 | il-central-1 | me-central-1 | me-south-1 | mx-central-1 | sa-east-1 | us-east-2 | us-gov-east-1 | us-gov-west-1 | us-west-1 | us-west-2
HTTP/1.1 200
Location: Location
x-amz-bucket-arn: BucketArn

            PUT / HTTP/1.1
            Host: amzn-s3-demo-bucket.s3.<Region>.amazonaws.com
            Content-Length: 0
            Date: Wed, 01 Mar  2006 12:00:00 GMT
            Authorization: authorization string
         
            HTTP/1.1 200 OK
            x-amz-id-2: YgIPIfBiKa2bj0KMg95r/0zo3emzU4dzsD4rcKCHQUAdQkf3ShJTOOpXUueF6QKo
            x-amz-request-id: 236A8905248E5A01
            Date: Wed, 01 Mar  2006 12:00:00 GMT

            Location: /amzn-s3-demo-bucket
            Content-Length: 0
            Connection: close
            Server: AmazonS3
         
            PUT / HTTP/1.1
            Host: amzn-s3-demo-bucket.s3.amazonaws.com
            Date: Wed, 12 Oct 2009 17:50:00 GMT
            Authorization: authorization string
            Content-Type: text/plain
            Content-Length: 124

            <CreateBucketConfiguration xmlns="http://s3.amazonaws.com/doc/2006-03-01/"> 
             <LocationConstraint>EU</LocationConstraint> 
            </CreateBucketConfiguration >
         
            PUT / HTTP/1.1
            Host: amzn-s3-demo-bucket.s3.<Region>.amazonaws.com
            Content-Length: 0
            x-amz-object-ownership: ObjectWriter
            Date: Tue, 30 Nov  2021 12:00:00 GMT
            Authorization: authorization string
         
            HTTP/1.1 200 OK
            x-amz-id-2: YgIPIfBiKa2bj0KMg95r/0zo3emzU4dzsD4rcKCHQUAdQkf3ShJTOOpXUueF6QKo
            x-amz-request-id: 236A8905248E5A01
            Date: Tue, 30 Nov  2021 12:00:00 GMT

            Location: /amzn-s3-demo-bucket
            Content-Length: 0
            Connection: close
            Server: AmazonS3
         
            PUT HTTP/1.1
            Host: amzn-s3-demo-bucket.s3.<Region>.amazonaws.com
            x-amz-date: Sat, 07 Apr 2012 00:54:40 GMT
            Authorization: authorization string
            x-amz-grant-write: id="79a59df900b949e55d96a1e698fbacedfd6e09d98eacf8f8d5218e7cd47ef2be", id="52b113e7a2f25102679df27bb0746532b8776c37c9fc7ca4ac94015369746b9f"
         
           HTTP/1.1 200 OK
         
            PUT / HTTP/1.1
            Host: amzn-s3-demo-bucket.s3.<Region>.amazonaws.com
            Content-Length: 0
            x-amz-acl: private
            Date: Wed, 01 Mar  2006 12:00:00 GMT
            Authorization: authorization string
         
            HTTP/1.1 200 OK
            x-amz-id-2: YgIPIfBiKa2bj0KMg95r/0zo3emzU4dzsD4rcKCHQUAdQkf3ShJTOOpXUueF6QKo
            x-amz-request-id: 236A8905248E5A01
            Date: Wed, 01 Mar  2006 12:00:00 GMT

            Location: /amzn-s3-demo-bucket
            Content-Length: 0
            Connection: close
            Server: AmazonS3
         
            PUT / HTTP/1.1
            Host: amzn-s3-demo-bucket-012345678910-us-west-2-an.s3.us-west-2.amazonaws.com
            Content-Length: 0
            x-amz-bucket-namespace: account-regional
            Date: Wed, 01 Mar  2006 12:00:00 GMT
            Authorization: authorization string
            <CreateBucketConfiguration xmlns="http://s3.amazonaws.com/doc/2006-03-01/">
             <LocationConstraint>us-west-2</LocationConstraint>
            </CreateBucketConfiguration >
         
            HTTP/1.1 200 OK
            x-amz-id-2: YgIPIfBiKa2bj0KMg95r/0zo3emzU4dzsD4rcKCHQUAdQkf3ShJTOOpXUueF6QKo
            x-amz-request-id: 236A8905248E5A01
            Date: Wed, 01 Mar  2006 12:00:00 GMT

            Location: /amzn-s3-demo-bucket-012345678910-us-west-2-an
            Content-Length: 0
            Connection: close
            Server: AmazonS3
         GET /?analytics&id=Id HTTP/1.1
Host: Bucket.s3.amazonaws.com
x-amz-expected-bucket-owner: ExpectedBucketOwner
URI Request Parameters

The request uses the following URI parameters.

Bucket
The name of the bucket from which an analytics configuration is retrieved.

Required: Yes

id
The ID that identifies the analytics configuration.

Required: Yes

x-amz-expected-bucket-owner
The account ID of the expected bucket owner. If the account ID that you provide does not match the actual owner of the bucket, the request fails with the HTTP status code 403 Forbidden (access denied).

Request Body

The request does not have a request body.

Response Syntax

HTTP/1.1 200
<?xml version="1.0" encoding="UTF-8"?>
<AnalyticsConfiguration>
   <Id>string</Id>
   <Filter>
      <And>
         <Prefix>string</Prefix>
         <Tag>
            <Key>string</Key>
            <Value>string</Value>
         </Tag>
         ...
      </And>
      <Prefix>string</Prefix>
      <Tag>
         <Key>string</Key>
         <Value>string</Value>
      </Tag>
   </Filter>
   <StorageClassAnalysis>
      <DataExport>
         <Destination>
            <S3BucketDestination>
               <Bucket>string</Bucket>
               <BucketAccountId>string</BucketAccountId>
               <Format>string</Format>
               <Prefix>string</Prefix>
            </S3BucketDestination>
         </Destination>
         <OutputSchemaVersion>string</OutputSchemaVersion>
      </DataExport>
   </StorageClassAnalysis>
</AnalyticsConfiguration>
Response Elements

If the action is successful, the service sends back an HTTP 200 response.

The following data is returned in XML format by the service.

AnalyticsConfiguration
Root level tag for the AnalyticsConfiguration parameters.

Required: Yes

Filter
The filter used to describe a set of objects for analyses. A filter must have exactly one prefix, one tag, or one conjunction (AnalyticsAndOperator). If no filter is provided, all objects will be considered in any analysis.

Type: AnalyticsFilter data type

Id
The ID that identifies the analytics configuration.

Type: String

StorageClassAnalysis
Contains data related to access patterns to be collected and made available to analyze the tradeoffs between different storage classes.

Type: StorageClassAnalysis data type

GET /?analytics&id=list1 HTTP/1.1
Host: amzn-s3-demo-bucket.s3.<Region>.amazonaws.com
Date: Mon, 31 Oct 2016 12:00:00 GMT
Authorization: authorization string
         
HTTP/1.1 200 OK
x-amz-id-2: YgIPIfBiKa2bj0KMgUAdQkf3ShJTOOpXUueF6QKo
x-amz-request-id: 236A8905248E5A02
Date: Mon, 31 Oct 2016 12:00:00 GMT
Server: AmazonS3
Content-Length: length

<?xml version="1.0" encoding="UTF-8"?>
<AnalyticsConfiguration xmlns="http://s3.amazonaws.com/doc/2006-03-01/">
  <Id>list1</Id>
  <Filter>
    <And>
      <Prefix>images/</Prefix>
      <Tag>
        <Key>dog</Key>
        <Value>corgi</Value>
      </Tag>
    </And>
  </Filter>
  <StorageClassAnalysis>
    <DataExport>
      <OutputSchemaVersion>V_1</OutputSchemaVersion>
      <Destination>
        <S3BucketDestination>
          <Format>CSV</Format>
          <BucketAccountId>123456789012</BucketAccountId>
          <Bucket>arn:aws:s3:::destination-bucket</Bucket>
          <Prefix>destination-prefix</Prefix>
        </S3BucketDestination>
      </Destination>
    </DataExport>
  </StorageClassAnalysis>
</AnalyticsConfiguration>
PUT /?website HTTP/1.1
Host: Bucket.s3.amazonaws.com
Content-MD5: ContentMD5
x-amz-sdk-checksum-algorithm: ChecksumAlgorithm
x-amz-expected-bucket-owner: ExpectedBucketOwner
<?xml version="1.0" encoding="UTF-8"?>
<WebsiteConfiguration xmlns="http://s3.amazonaws.com/doc/2006-03-01/">
   <ErrorDocument>
      <Key>string</Key>
   </ErrorDocument>
   <IndexDocument>
      <Suffix>string</Suffix>
   </IndexDocument>
   <RedirectAllRequestsTo>
      <HostName>string</HostName>
      <Protocol>string</Protocol>
   </RedirectAllRequestsTo>
   <RoutingRules>
      <RoutingRule>
         <Condition>
            <HttpErrorCodeReturnedEquals>string</HttpErrorCodeReturnedEquals>
            <KeyPrefixEquals>string</KeyPrefixEquals>
         </Condition>
         <Redirect>
            <HostName>string</HostName>
            <HttpRedirectCode>string</HttpRedirectCode>
            <Protocol>string</Protocol>
            <ReplaceKeyPrefixWith>string</ReplaceKeyPrefixWith>
            <ReplaceKeyWith>string</ReplaceKeyWith>
         </Redirect>
      </RoutingRule>
   </RoutingRules>
</WebsiteConfiguration>           
URI Request Parameters

The request uses the following URI parameters.

Bucket
The bucket name.

Required: Yes

Content-MD5
The Base64 encoded 128-bit MD5 digest of the data. You must use this header as a message integrity check to verify that the request body was not corrupted in transit. For more information, see RFC 1864.

For requests made using the AWS Command Line Interface (CLI) or AWS SDKs, this field is calculated automatically.

x-amz-expected-bucket-owner
The account ID of the expected bucket owner. If the account ID that you provide does not match the actual owner of the bucket, the request fails with the HTTP status code 403 Forbidden (access denied).

x-amz-sdk-checksum-algorithm
Indicates the algorithm used to create the checksum for the request when you use the SDK. This header will not provide any additional functionality if you don't use the SDK. When you send this header, there must be a corresponding x-amz-checksum or x-amz-trailer header sent. Otherwise, Amazon S3 fails the request with the HTTP status code 400 Bad Request. For more information, see Checking object integrity in the Amazon S3 User Guide.

If you provide an individual checksum, Amazon S3 ignores any provided ChecksumAlgorithm parameter.

Valid Values: CRC32 | CRC32C | SHA1 | SHA256 | CRC64NVME | SHA512 | MD5 | XXHASH64 | XXHASH3 | XXHASH128

Request Body

The request accepts the following data in XML format.

WebsiteConfiguration
Root level tag for the WebsiteConfiguration parameters.

Required: Yes

ErrorDocument
The name of the error document for the website.

Type: ErrorDocument data type

Required: No

IndexDocument
The name of the index document for the website.

Type: IndexDocument data type

Required: No

RedirectAllRequestsTo
The redirect behavior for every request to this bucket's website endpoint.

Important
If you specify this property, you can't specify any other property.

Type: RedirectAllRequestsTo data type

Required: No

RoutingRules
Rules that define when a redirect is applied and the redirect behavior.

Type: Array of RoutingRule data types

Required: No

Response Syntax

HTTP/1.1 200
Response Elements

If the action is successful, the service sends back an HTTP 200 response with an empty HTTP body.

PUT ?website HTTP/1.1
Host: example.com.s3.<Region>.amazonaws.com
Content-Length: 256
Date: Thu, 27 Jan 2011 12:00:00 GMT
Authorization: signatureValue

<WebsiteConfiguration xmlns='http://s3.amazonaws.com/doc/2006-03-01/'>
    <IndexDocument>
        <Suffix>index.html</Suffix>
    </IndexDocument>
    <ErrorDocument>
        <Key>SomeErrorDocument.html</Key>
    </ErrorDocument>
</WebsiteConfiguration>
         
HTTP/1.1 200 OK
x-amz-id-2: YgIPIfBiKa2bj0KMgUAdQkf3ShJTOOpXUueF6QKo
x-amz-request-id: 80CD4368BD211111
Date: Thu, 27 Jan 2011 00:00:00 GMT
Content-Length: 0
Server: AmazonS3
  
PUT ?website HTTP/1.1
Host: www.example.com.s3.<Region>.amazonaws.com
Content-Length: length-value
Date: Thu, 27 Jan 2011 12:00:00 GMT
Authorization: signatureValue

<WebsiteConfiguration xmlns='http://s3.amazonaws.com/doc/2006-03-01/'>
   <RedirectAllRequestsTo>
      <HostName>example.com</HostName>
    </RedirectAllRequestsTo>
</WebsiteConfiguration>
          
PUT ?website HTTP/1.1
Host: www.example.com.s3.<Region>.amazonaws.com
Content-Length: length-value
Date: Thu, 27 Jan 2011 12:00:00 GMT
Authorization: signatureValue

<WebsiteConfiguration xmlns='http://s3.amazonaws.com/doc/2006-03-01/'>
  <IndexDocument>
    <Suffix>index.html</Suffix>
  </IndexDocument>
  <ErrorDocument>
    <Key>Error.html</Key>
  </ErrorDocument>

  <RoutingRules>
    <RoutingRule>
    <Condition>
      <KeyPrefixEquals>docs/</KeyPrefixEquals>
    </Condition>
    <Redirect>
      <ReplaceKeyPrefixWith>documents/</ReplaceKeyPrefixWith>
    </Redirect>
    </RoutingRule>
  </RoutingRules>
</WebsiteConfiguration>

PUT ?website HTTP/1.1
Host: www.example.com.s3.<Region>.amazonaws.com
Content-Length: 580
Date: Thu, 27 Jan 2011 12:00:00 GMT
Authorization: signatureValue

<WebsiteConfiguration xmlns='http://s3.amazonaws.com/doc/2006-03-01/'>
  <IndexDocument>
    <Suffix>index.html</Suffix>
  </IndexDocument>
  <ErrorDocument>
    <Key>Error.html</Key>
  </ErrorDocument>

  <RoutingRules>
    <RoutingRule>
    <Condition>
      <HttpErrorCodeReturnedEquals>404</HttpErrorCodeReturnedEquals >
    </Condition>
    <Redirect>
      <HostName>ec2-11-22-333-44.compute-1.amazonaws.com</HostName>
      <ReplaceKeyPrefixWith>report-404/</ReplaceKeyPrefixWith>
    </Redirect>
    </RoutingRule>
  </RoutingRules>
</WebsiteConfiguration>
  
PUT ?website HTTP/1.1
Host: www.example.com.s3.<Region>.amazonaws.com
Content-Length: 481
Date: Thu, 27 Jan 2011 12:00:00 GMT
Authorization: signatureValue

<WebsiteConfiguration xmlns='http://s3.amazonaws.com/doc/2006-03-01/'>
  <IndexDocument>
    <Suffix>index.html</Suffix>
  </IndexDocument>
  <ErrorDocument>
    <Key>Error.html</Key>
  </ErrorDocument>

  <RoutingRules>
    <RoutingRule>
    <Condition>
      <KeyPrefixEquals>images/</KeyPrefixEquals>
    </Condition>
    <Redirect>
      <ReplaceKeyWith>errorpage.html</ReplaceKeyWith>
    </Redirect>
    </RoutingRule>
  </RoutingRules>
</WebsiteConfiguration>
 npx wrangler r2 bucket create <dark-worker>
npx wrangler r2 bucket create <loop>
npx wrangler r2 bucket create <run-19>
npx wrangler r2 bucket create <run-20>
npx wrangler r2 bucket create <run-21>
npx wrangler r2 bucket create <run-22>
npx wrangler r2 bucket create <run-23>
npx wrangler r2 bucket create <run-24>
npx wrangler r2 bucket create <run-26>
npx wrangler r2 bucket create <run-25>
npx wrangler r2 bucket create <run-3>
npx wrangler r2 bucket create <run1>
<NAME>string required
The name of the new bucket

--locationstring
The optional location hint that determines geographic placement of the R2 bucket

--storage-classstring alias: --s
The default storage class for objects uploaded to this bucket

--jurisdictionstring alias: --J
The jurisdiction where the new bucket will be created

--use-remoteboolean
Use a remote binding when adding the newly created resource to your config

--update-configboolean
Automatically update your config file with the newly added resource

--bindingstring
The binding name of this resource in your Worker
Global flags
r2 bucket info
Get information about an R2 bucket
npx wrangler r2 bucket info <dark-worker>
npx wrangler r2 bucket info <loop>
npx wrangler r2 bucket info <run-19>
npx wrangler r2 bucket info <run-20>
npx wrangler r2 bucket info <run-21>
npx wrangler r2 bucket info <run-22>
npx wrangler r2 bucket info <run-23>
npx wrangler r2 bucket info <run-24>
npx wrangler r2 bucket info <run-26>
npx wrangler r2 bucket info <run-25>
npx wrangler r2 bucket info <run-3>
npx wrangler r2 bucket info <run1>
<BUCKET>string required
The name of the bucket to retrieve info for

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

--jsonboolean default: false
Return the bucket information as JSON

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
Use a specific auth profile
npx wrangler r2 bucket domain update <dark-worker>
npx wrangler r2 bucket domain update <loop>
npx wrangler r2 bucket domain update <run-19>
npx wrangler r2 bucket domain update <run-20>
npx wrangler r2 bucket domain update <run-21>
npx wrangler r2 bucket domain update <run-22>
npx wrangler r2 bucket domain update <run-23>
npx wrangler r2 bucket domain update <run-24>
npx wrangler r2 bucket domain update <run-26>
npx wrangler r2 bucket domain update <run-25>
npx wrangler r2 bucket domain update <run-3>
npx wrangler r2 bucket domain update <run1>
<BUCKET>string required
The name of the R2 bucket associated with the custom domain to update

--domainstring required
The custom domain whose settings will be updated

--min-tlsstring
Update the minimum TLS version for the custom domain

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
npx wrangler r2 bucket domain get <dark-worker>
npx wrangler r2 bucket domain get <loop>
npx wrangler r2 bucket domain get <run-19>
npx wrangler r2 bucket domain get <run-20>
npx wrangler r2 bucket domain get <run-21>
npx wrangler r2 bucket domain get <run-22>
npx wrangler r2 bucket domain get <run-23>
npx wrangler r2 bucket domain get <run-24>
npx wrangler r2 bucket domain get <run-25>
npx wrangler r2 bucket domain get <run-3>
npx wrangler r2 bucket domain get <run1>
<BUCKET>string required
The name of the R2 bucket whose custom domain to retrieve

--domainstring required
The custom domain to get information for

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
BUCKET_BINDING.list(objects Array<R2Object>)
BUCKET_BINDING.list(truncated Boolean)
BUCKET_BINDING.list(cursor string)
BUCKET_BINDING.list(delimitedPrefixes Array<string>)
get(etagMatches string)
get(etagDoesNotMatch string)
get(uploadedBefore Date)
get(uploadedAfter Date)
get(contentType string)
get(contentLanguage string)
get(contentDisposition string)
get(contentEncoding string)
get(cacheControl string)
get(cacheExpiry Date)
put(md5 ArrayBuffer)
put(sha1 ArrayBuffer)
put(sha256 ArrayBuffer)
put(sha384 ArrayBuffer)
put(sha512 ArrayBuffer)
R2UploadedPart(etag)
Use a specific auth profile
npx wrangler basin catalog snapshot-expiration enable <dark-worker>
npx wrangler basin catalog compaction disable my-bucket my-namespace my-table
npx wrangler basin catalog snapshot-expiration enable <loop>
npx wrangler basin catalog compaction disable my-bucket my-namespace my-table
npx wrangler basin catalog snapshot-expiration enable <run-19>
npx wrangler basin catalog compaction disable my-bucket my-namespace my-table
npx wrangler basin catalog snapshot-expiration enable <run-20>
npx wrangler basin catalog compaction disable my-bucket my-namespace my-table
npx wrangler basin catalog snapshot-expiration enable <run-21>
npx wrangler basin catalog compaction disable my-bucket my-namespace my-table
npx wrangler basin catalog snapshot-expiration enable <run-22>
npx wrangler basin catalog compaction disable my-bucket my-namespace my-table
npx wrangler basin catalog snapshot-expiration enable <run-23>
npx wrangler basin catalog compaction disable my-bucket my-namespace my-table
npx wrangler basin catalog snapshot-expiration enable <run-24>
npx wrangler basin catalog compaction disable my-bucket my-namespace my-table
npx wrangler basin catalog snapshot-expiration enable <run-25>
npx wrangler basin catalog compaction disable my-bucket my-namespace my-table
npx wrangler basin catalog snapshot-expiration enable <run-3>
npx wrangler basin catalog compaction disable my-bucket my-namespace my-table

npx wrangler basin catalog snapshot-expiration enable <run1>
npx wrangler basin catalog compaction disable my-bucket my-namespace my-table
npx wrangler r2 bucket cors set <dark-worker>
npx wrangler r2 bucket cors set <loop>
npx wrangler r2 bucket cors set <run-19>
npx wrangler r2 bucket cors set <run-20>
npx wrangler r2 bucket cors set <run-21>
npx wrangler r2 bucket cors set <run-22>
npx wrangler r2 bucket cors set <run-23>
npx wrangler r2 bucket cors set <run-24>
npx wrangler r2 bucket cors set <run-25>
npx wrangler r2 bucket cors set <run-3>
npx wrangler r2 bucket cors set <run1>
<BUCKET>string required
The name of the R2 bucket to set the CORS configuration for

--filestring required
Path to the JSON file containing the CORS configuration

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

--forceboolean alias: --ydefault: false
Skip confirmation

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
Use a specific auth profile
npx wrangler r2 bucket dev-url enable <dark-worker>
npx wrangler r2 bucket dev-url enable <loop>
npx wrangler r2 bucket dev-url enable <run-19>
npx wrangler r2 bucket dev-url enable <run-20>
npx wrangler r2 bucket dev-url enable <run-21>
npx wrangler r2 bucket dev-url enable <run-22>
npx wrangler r2 bucket dev-url enable <run-23>
npx wrangler r2 bucket dev-url enable <run-24>
npx wrangler r2 bucket dev-url enable <run-25>
npx wrangler r2 bucket dev-url enable <run-3>
npx wrangler r2 bucket dev-url enable <run1>
<BUCKET>string required
The name of the R2 bucket to disable public access via its r2.dev URL

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

--forceboolean alias: --ydefault: false
Skip confirmation

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
Use a specific auth profile
npx wrangler r2 bucket dev-url get <dark-worker>
npx wrangler r2 bucket dev-url get <loop>
npx wrangler r2 bucket dev-url get <run-19>
npx wrangler r2 bucket dev-url get <run-20>
npx wrangler r2 bucket dev-url get <run-21>
npx wrangler r2 bucket dev-url get <run-22>
npx wrangler r2 bucket dev-url get <run-23>
npx wrangler r2 bucket dev-url get <run-24>
npx wrangler r2 bucket dev-url get <run-25>
npx wrangler r2 bucket dev-url get <run-3>
npx wrangler r2 bucket dev-url get <run1>
<BUCKET>string required
The name of the R2 bucket whose r2.dev URL status to retrieve

--jurisdictionstring alias: --J
The jurisdiction where the bucket exists

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
Use a specific auth profile
npx wrangler r2 object put <OBJECTPATH>

<OBJECTPATH>string required
The destination object path in the form of {bucket}/{key}

--content-typestring alias: --ct
A standard MIME type describing the format of the object data

--content-dispositionstring alias: --cd
Specifies presentational information for the object

--content-encodingstring alias: --ce
Specifies what content encodings have been applied to the object and thus what decoding mechanisms must be applied to obtain the media-type referenced by the Content-Type header field

--content-languagestring alias: --cl
The language the content is in

--cache-controlstring alias: --cc
Specifies caching behavior along the request/reply chain

--expiresstring
The date and time at which the object is no longer cacheable

--localboolean
Interact with local storage

--remoteboolean
Interact with remote storage

--persist-tostring
Directory for local persistence

--jurisdictionstring alias: --J
The jurisdiction where the object will be created

--storage-classstring alias: --s
The storage class of the object to be created

--forceboolean alias: --ydefault: false
Skip data catalog validation prompt

--filestring alias: --f
The path of the file to upload

--pipeboolean alias: --p
Enables the file to be piped in, rather than specified with the --file option

Global flags
--vboolean alias: --version
Show version number

--cwdstring
Run as if Wrangler was started in the specified directory instead of the current working directory

--configstring alias: --c
Path to Wrangler configuration file

--envstring alias: --e
Environment to use for operations, and for selecting .env and .dev.vars files

--env-filestring
Path to an .env file to load - can be specified multiple times - values from earlier files are overridden by values in later files

--install-skillsboolean default: false
Install Cloudflare skills for detected AI coding agents before running the command

--profilestring
Use a specific auth profile
(Tysavmedianav@gmail.com)
$ErrorActionPreference = "Stop"

# TYSAV Cloudflare R2 bucket export

$Buckets = @(
"dark-worker",
"loop",
"run-19",
"run-20",
"run-21",
"run-22",
"run-23",
"run-24",
"run-25",
"run-3",
"run1"
)

$R2Remote = "tysav-r2"
$ExportRoot = "C:\TYSAV\BucketExports"
$LogDirectory = "C:\TYSAV\Logs"

New-Item -ItemType Directory -Force -Path $ExportRoot |
Out-Null

New-Item -ItemType Directory -Force -Path $LogDirectory |
Out-Null

$LogFile = Join-Path $LogDirectory "TYSAV-Bucket-Export.log"

function Write-Log {
param([string]$Message)

```
$Line = "$(Get-Date -Format o) $Message"
Add-Content -LiteralPath $LogFile -Value $Line
Write-Host $Line
```

}

$Failures = 0

foreach ($Bucket in $Buckets) {
$Destination = Join-Path $ExportRoot $Bucket

```
New-Item -ItemType Directory -Force -Path $Destination |
    Out-Null

Write-Log "START bucket=$Bucket"

try {
    # Download objects from Cloudflare R2.
    # No delete operation is performed.
    & rclone copy `
        "${R2Remote}:$Bucket" `
        $Destination `
        --create-empty-src-dirs `
        --retries 3 `
        --low-level-retries 5

    if ($LASTEXITCODE -ne 0) {
        throw "Rclone export failed with exit code $LASTEXITCODE"
    }

    # Create a SHA-256 manifest for exported files.
    $Manifest = Join-Path $Destination "SHA256-MANIFEST.csv"

    Get-ChildItem -LiteralPath $Destination -File -Recurse |
        Where-Object { $_.FullName -ne $Manifest } |
        ForEach-Object {
            $Hash = Get-FileHash `
                -LiteralPath $_.FullName `
                -Algorithm SHA256

            [PSCustomObject]@{
                Bucket       = $Bucket
                RelativePath = $_.FullName.Substring(
                    $Destination.Length
                ).TrimStart('\')
                SizeBytes    = $_.Length
                SHA256       = $Hash.Hash
            }
        } |
        Export-Csv -LiteralPath $Manifest -NoTypeInformation

    Write-Log "SUCCESS bucket=$Bucket destination=$Destination"
}
catch {
    $Failures++
    Write-Log "FAILED bucket=$Bucket error=$($_.Exception.Message)"
}
```

}

Write-Log "EXPORT COMPLETE failures=$Failures total=$($Buckets.Count)"

if ($Failures -gt 0) {
exit 1
}
rclone config
https://YOUR_ACCOUNT_ID.r2.cloudflarestorage.com
winget install Rclone.Rclone
rclone config
rclone lsd tysav-gdrive:
C:\TYSAV\Export-TYSAV-To-GoogleDrive.ps1
$ErrorActionPreference = "Stop"

# TYSAV Cloudflare R2 -> Google Drive export

$Buckets = @(
"dark-worker",
"loop",
"run-19",
"run-20",
"run-21",
"run-22",
"run-23",
"run-24",
"run-25",
"run-3",
"run1"
)

$R2Remote = "tysav-r2"
$GoogleDriveRemote = "tysav-gdrive"

$Root = "C:\TYSAV"
$ExportRoot = Join-Path $Root "BucketExports"
$LogDirectory = Join-Path $Root "Logs"

$Timestamp = (Get-Date).ToUniversalTime().ToString(
"yyyy-MM-ddTHH-mm-ssZ"
)

$DriveRoot = "TYSAV-Cloudflare-Exports/$Timestamp"
$LogFile = Join-Path $LogDirectory "TYSAV-GoogleDrive-Export.log"

New-Item -ItemType Directory -Force -Path $ExportRoot |
Out-Null

New-Item -ItemType Directory -Force -Path $LogDirectory |
Out-Null

function Write-Log {
param([string]$Message)

```
$Line = "$(Get-Date -Format o) $Message"

Add-Content -LiteralPath $LogFile -Value $Line
Write-Host $Line
```

}

# Verify both connections before processing buckets.

& rclone lsd "${R2Remote}:"

if ($LASTEXITCODE -ne 0) {
throw "Cannot access Cloudflare R2 remote: $R2Remote"
}

& rclone lsd "${GoogleDriveRemote}:"

if ($LASTEXITCODE -ne 0) {
throw "Cannot access Google Drive remote: $GoogleDriveRemote"
}

$Failures = 0

foreach ($Bucket in $Buckets) {
$LocalPath = Join-Path $ExportRoot "$Timestamp$Bucket"
$DrivePath = "${GoogleDriveRemote}:$DriveRoot/$Bucket"

```
New-Item -ItemType Directory -Force -Path $LocalPath |
    Out-Null

Write-Log "START bucket=$Bucket"

try {
    # 1. Download from Cloudflare R2.
    & rclone copy `
        "${R2Remote}:$Bucket" `
        $LocalPath `
        --create-empty-src-dirs `
        --retries 3 `
        --low-level-retries 5

    if ($LASTEXITCODE -ne 0) {
        throw "R2 download failed."
    }

    # 2. Generate a SHA-256 manifest.
    $Manifest = Join-Path $LocalPath "SHA256-MANIFEST.csv"

    $Files = @(
        Get-ChildItem `
            -LiteralPath $LocalPath `
            -File `
            -Recurse |
        Where-Object { $_.FullName -ne $Manifest }
    )

    $ManifestRows = foreach ($File in $Files) {
        $Hash = Get-FileHash `
            -LiteralPath $File.FullName `
            -Algorithm SHA256

        [PSCustomObject]@{
            Bucket = $Bucket
            RelativePath = $File.FullName.Substring(
                $LocalPath.Length
            ).TrimStart('\')
            SizeBytes = $File.Length
            SHA256 = $Hash.Hash
        }
    }

    $ManifestRows |
        Export-Csv `
            -LiteralPath $Manifest `
            -NoTypeInformation

    # 3. Upload files and manifest to Google Drive.
    & rclone copy `
        $LocalPath `
        $DrivePath `
        --create-empty-src-dirs `
        --retries 3 `
        --low-level-retries 5

    if ($LASTEXITCODE -ne 0) {
        throw "Google Drive upload failed."
    }

    # 4. Verify the uploaded files against the local export.
    & rclone check $LocalPath $DrivePath

    if ($LASTEXITCODE -ne 0) {
        throw "Google Drive verification failed."
    }

    Write-Log "SUCCESS bucket=$Bucket drive=$DrivePath"
}
catch {
    $Failures++

    Write-Log (
        "FAILED bucket=$Bucket error=" +
        $_.Exception.Message
    )
}
```

}

Write-Log (
"EXPORT FINISHED total=$($Buckets.Count) " +
"failures=$Failures timestamp=$Timestamp"
)

if ($Failures -gt 0) {
exit 1
}
$ScriptPath = "C:\TYSAV\Export-TYSAV-To-GoogleDrive.ps1"

$Action = New-ScheduledTaskAction `
    -Execute "powershell.exe" `
    -Argument "-NoProfile -NonInteractive -ExecutionPolicy Bypass -File `"$ScriptPath`""

$Trigger = New-ScheduledTaskTrigger `
    -Once `
    -At (Get-Date).AddMinutes(2)

$Trigger.Repetition.Interval = "PT30M"
$Trigger.Repetition.Duration = "P1D"

$Settings = New-ScheduledTaskSettingsSet `
    -StartWhenAvailable `
    -MultipleInstances IgnoreNew

Register-ScheduledTask `
    -TaskName "TYSAV-GoogleDrive-Bucket-Export" `
    -Action $Action `
    -Trigger $Trigger `
    -Settings $Settings `
    -Description "Export 11 Cloudflare R2 buckets to Google Drive every 30 minutes." `
    -Force
